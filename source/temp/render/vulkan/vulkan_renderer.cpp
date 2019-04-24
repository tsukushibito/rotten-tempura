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
  camera_table_ = make_shared<CameraTable>();
  camera_table_mutex_ = make_shared<mutex>();
}

void VulkanRenderer::Render() {
  using namespace std;
  using namespace gfx::vulkan;
  unique_lock<mutex> lock(*camera_table_mutex_);
  for (auto&& camera : *camera_table_) {
    auto&& vk_swap_chain =
        static_pointer_cast<VulkanSwapChain>(camera->swap_chain);
  }
}
}  // namespace vulkan
}  // namespace render
}  // namespace temp