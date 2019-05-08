#pragma once

#include "temp/render/vulkan/vulkan_renderer.h"
#include "temp/render/camera.h"

#include "temp/gfx/vulkan/vulkan_swap_chain.h"

namespace temp {
namespace render {
namespace vulkan {

namespace {
vk::UniqueRenderPass CreateRenderPass(const vk::Device vk_device,
                                      vk::Format color_format) {
  vk::AttachmentDescription color_attachment;
  color_attachment.format = color_format;
  color_attachment.samples = vk::SampleCountFlagBits::e1;
  color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
  color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
  color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
  color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
  color_attachment.initialLayout = vk::ImageLayout::eUndefined;
  color_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

  vk::AttachmentReference color_attachment_ref;
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

  vk::SubpassDescription subpass;
  subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  vk::RenderPassCreateInfo render_pass_ci;
  vk::SubpassDependency subpass_dependency;
  subpass_dependency.dstSubpass = 0;
  subpass_dependency.srcStageMask =
      vk::PipelineStageFlagBits::eColorAttachmentOutput;
  subpass_dependency.srcAccessMask = vk::AccessFlagBits();
  subpass_dependency.dstStageMask =
      vk::PipelineStageFlagBits::eColorAttachmentOutput;
  subpass_dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead |
                                     vk::AccessFlagBits::eColorAttachmentWrite;
  render_pass_ci.attachmentCount = 1;
  render_pass_ci.pAttachments = &color_attachment;
  render_pass_ci.subpassCount = 1;
  render_pass_ci.pSubpasses = &subpass;
  render_pass_ci.dependencyCount = 1;
  render_pass_ci.pDependencies = &subpass_dependency;
  return vk_device.createRenderPassUnique(render_pass_ci);
}
}  // namespace

VulkanRenderer::VulkanRenderer(const std::shared_ptr<gfx::Device>& device) {
  using namespace std;
  using namespace gfx::vulkan;
  device_ = static_pointer_cast<VulkanDevice>(device);
  auto vk_device = device_->device();

  vk::CommandPoolCreateInfo command_pool_ci;
  auto pair = device_->queue_index_table().find(vk::QueueFlagBits::eGraphics);
  command_pool_ci.queueFamilyIndex = pair->second;
  command_pool_ = vk_device.createCommandPoolUnique(command_pool_ci);
}

void VulkanRenderer::Render() {
  using namespace std;
  using namespace gfx::vulkan;

  primary_command_buffers_.clear();

  camera_manager_->Foreach([this](Camera camera) {
    if (camera.swap_chain != nullptr) {
      vk::CommandBufferAllocateInfo command_buffer_ai;
      command_buffer_ai.commandPool = *command_pool_;
      command_buffer_ai.level = vk::CommandBufferLevel::ePrimary;
      command_buffer_ai.commandBufferCount = 1;
      auto command_buffers =
          device_->device().allocateCommandBuffersUnique(command_buffer_ai);

      auto&& command_buffer = command_buffers[0];

      auto swap_chain = static_pointer_cast<VulkanSwapChain>(camera.swap_chain);

      vk::CommandBufferBeginInfo command_buffer_bi;
      command_buffer_bi.flags =
          vk::CommandBufferUsageFlagBits::eSimultaneousUse;
      command_buffer->begin(command_buffer_bi);

      vk::RenderPassBeginInfo render_pass_bi;
      render_pass_bi.renderPass = swap_chain->render_pass();
      render_pass_bi.renderArea.offset = vk::Offset2D{0, 0};
      render_pass_bi.renderArea.extent =
          vk::Extent2D{swap_chain->width(), swap_chain->height()};
      vk::ClearValue clear_value;
      auto&& c = camera.clear_color;
      clear_value.color.setFloat32({c.red, c.green, c.blue, c.alpha});
      render_pass_bi.clearValueCount = 1;
      render_pass_bi.pClearValues = &clear_value;
      render_pass_bi.framebuffer = swap_chain->current_frame_buffer();

      command_buffer->beginRenderPass(render_pass_bi,
                                      vk::SubpassContents::eInline);

      command_buffer->endRenderPass();
      command_buffer->end();

      primary_command_buffers_.insert(
          primary_command_buffers_.end(),
          make_move_iterator(command_buffers.begin()),
          make_move_iterator(command_buffers.end()));
    }
    camera.clear_color;
  });
  camera_manager_->RemoveUnusedObjects();
}
}  // namespace vulkan
}  // namespace render
}  // namespace temp