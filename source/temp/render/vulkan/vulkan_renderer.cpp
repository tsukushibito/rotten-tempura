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
  camera_manager_->Foreach([this](Camera camera) {
    if (camera.swap_chain != nullptr) {
      auto swap_chain = static_pointer_cast<VulkanSwapChain>(camera.swap_chain);
      auto current_image_index = swap_chain->current_image_index();
      auto frame_buffer = swap_chain->frame_buffer(current_image_index);
    }
    camera.clear_color;
  });
  camera_manager_->RemoveUnusedObjects();
}
}  // namespace vulkan
}  // namespace render
}  // namespace temp