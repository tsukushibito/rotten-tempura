#include <cstdlib>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <temp/app/app.h>
#include <temp/core/core.h>
#include <temp/gfx/gfx.h>

#include <temp/gfx/tvk/tvk_context.h>
#include <temp/gfx/tvk/tvk_device.h>
#include <temp/gfx/tvk/tvk_swap_chain.h>

#include <boost/filesystem.hpp>

using namespace temp;

std::vector<char> ReadFile(const std::string& file_path) {
  std::ifstream fs(file_path, std::ios::ate | std::ios::binary);

  if (!fs.is_open()) {
    TEMP_LOG_ERROR("Failed to open file! : ", file_path);
    return std::vector<char>();
  }

  auto file_size = (std::size_t)fs.tellg();
  std::vector<char> buf(file_size);

  fs.seekg(0);
  fs.read(buf.data(), file_size);

  fs.close();

  return buf;
}

class TestRenderer {
 public:
  TestRenderer(const std::shared_ptr<gfx::Device>& device) {
    tvk_device_ = std::static_pointer_cast<gfx::tvk::TvkDevice>(device);
    auto vk_device = tvk_device_->device();

    auto swap_chain = device->main_swap_chain();
    auto tvk_swap_chain = static_cast<gfx::tvk::TvkSwapChain*>(swap_chain);

    auto vs_code = ReadFile("shader/vert.spv");

    vk::ShaderModuleCreateInfo vs_module_ci;
    vs_module_ci.codeSize = vs_code.size();
    vs_module_ci.pCode = (std::uint32_t*)vs_code.data();
    vs_module_ = vk_device.createShaderModuleUnique(vs_module_ci);

    vk::PipelineShaderStageCreateInfo vs_stage_ci;
    vs_stage_ci.stage = vk::ShaderStageFlagBits::eVertex;
    vs_stage_ci.module = *vs_module_;
    vs_stage_ci.pName = "main";

    auto fs_code = ReadFile("shader/frag.spv");

    vk::ShaderModuleCreateInfo fs_module_ci;
    fs_module_ci.codeSize = fs_code.size();
    fs_module_ci.pCode = (std::uint32_t*)fs_code.data();
    auto fs_module_ = vk_device.createShaderModuleUnique(fs_module_ci);

    vk::PipelineShaderStageCreateInfo fs_stage_ci;
    fs_stage_ci.stage = vk::ShaderStageFlagBits::eFragment;
    fs_stage_ci.module = *fs_module_;
    fs_stage_ci.pName = "main";

    vk::PipelineShaderStageCreateInfo shader_stages[] = {vs_stage_ci,
                                                         fs_stage_ci};

    vk::PipelineVertexInputStateCreateInfo vertex_input_ci;
    vertex_input_ci.vertexBindingDescriptionCount = 0;
    vertex_input_ci.pVertexBindingDescriptions = nullptr;  // Optional
    vertex_input_ci.vertexAttributeDescriptionCount = 0;
    vertex_input_ci.pVertexAttributeDescriptions = nullptr;  // Optional

    vk::PipelineInputAssemblyStateCreateInfo input_assembly_ci;
    input_assembly_ci.topology = vk::PrimitiveTopology::eTriangleList;
    input_assembly_ci.primitiveRestartEnable = false;

    vk::Viewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = swap_chain->width();
    viewport.height = swap_chain->height();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vk::Rect2D scissor;
    scissor.offset = vk::Offset2D{0, 0};
    scissor.extent = vk::Extent2D{swap_chain->width(), swap_chain->height()};

    vk::PipelineViewportStateCreateInfo viewport_state_ci;
    viewport_state_ci.viewportCount = 1;
    viewport_state_ci.pViewports = &viewport;
    viewport_state_ci.scissorCount = 1;
    viewport_state_ci.pScissors = &scissor;

    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = false;
    rasterizer.rasterizerDiscardEnable = false;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasClamp = false;

    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = false;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = false;
    multisampling.alphaToOneEnable = false;

    vk::PipelineColorBlendAttachmentState color_blend_attachment;
    color_blend_attachment.colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eOne;
    color_blend_attachment.dstColorBlendFactor = vk::BlendFactor::eZero;
    color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    color_blend_attachment.alphaBlendOp = vk::BlendOp::eAdd;

    vk::PipelineColorBlendStateCreateInfo color_blending;
    color_blending.logicOpEnable = false;
    color_blending.logicOp = vk::LogicOp::eCopy;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;
    color_blending.blendConstants[0] = 0.0f;
    color_blending.blendConstants[1] = 0.0f;
    color_blending.blendConstants[2] = 0.0f;
    color_blending.blendConstants[3] = 0.0f;

    vk::DynamicState dynamic_states[] = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eLineWidth,
    };

    vk::PipelineDynamicStateCreateInfo dynamic_state;
    dynamic_state.dynamicStateCount = 2;
    dynamic_state.pDynamicStates = dynamic_states;

    vk::PipelineLayoutCreateInfo pipeline_layout_ci;
    pipeline_layout_ = vk_device.createPipelineLayoutUnique(pipeline_layout_ci);

    vk::AttachmentDescription color_attachment;
    color_attachment.format = tvk_swap_chain->color_format();
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
    subpass_dependency.dstAccessMask =
        vk::AccessFlagBits::eColorAttachmentRead |
        vk::AccessFlagBits::eColorAttachmentWrite;
    render_pass_ci.attachmentCount = 1;
    render_pass_ci.pAttachments = &color_attachment;
    render_pass_ci.subpassCount = 1;
    render_pass_ci.pSubpasses = &subpass;
    render_pass_ci.dependencyCount = 1;
    render_pass_ci.pDependencies = &subpass_dependency;
    render_pass_ = vk_device.createRenderPassUnique(render_pass_ci);

    vk::GraphicsPipelineCreateInfo pipeline_ci;
    pipeline_ci.stageCount = 2;
    pipeline_ci.pStages = shader_stages;
    pipeline_ci.pVertexInputState = &vertex_input_ci;
    pipeline_ci.pInputAssemblyState = &input_assembly_ci;
    pipeline_ci.pViewportState = &viewport_state_ci;
    pipeline_ci.pRasterizationState = &rasterizer;
    pipeline_ci.pMultisampleState = &multisampling;
    pipeline_ci.pDepthStencilState = nullptr;
    pipeline_ci.pColorBlendState = &color_blending;
    pipeline_ci.pDynamicState = nullptr;
    pipeline_ci.layout = *pipeline_layout_;
    pipeline_ci.renderPass = *render_pass_;
    pipeline_ci.subpass = 0;

    pipeline_ = vk_device.createGraphicsPipelineUnique(vk::PipelineCache(),
                                                       pipeline_ci);

    auto& images = tvk_swap_chain->images();

    for (int i = 0; i < images.size(); ++i) {
      vk::ImageView attachments[] = {*images[i].view};

      vk::FramebufferCreateInfo frame_buffer_ci;
      frame_buffer_ci.renderPass = *render_pass_;
      frame_buffer_ci.attachmentCount = 1;
      frame_buffer_ci.pAttachments = attachments;
      frame_buffer_ci.width = swap_chain->width();
      frame_buffer_ci.height = swap_chain->height();
      frame_buffer_ci.layers = 1;

      frame_buffers_.emplace_back(
          vk_device.createFramebufferUnique(frame_buffer_ci));
    }

    vk::CommandPoolCreateInfo command_pool_ci;
    auto pair = tvk_device_->context().queue_index_table().find(
        vk::QueueFlagBits::eGraphics);
    command_pool_ci.queueFamilyIndex = pair->second;
    command_pool_ = vk_device.createCommandPoolUnique(command_pool_ci);

    vk::CommandBufferAllocateInfo command_buffer_ai;
    command_buffer_ai.commandPool = *command_pool_;
    command_buffer_ai.level = vk::CommandBufferLevel::ePrimary;
    command_buffer_ai.commandBufferCount = images.size();
    command_buffers_ =
        vk_device.allocateCommandBuffersUnique(command_buffer_ai);

    for (int i = 0; i < command_buffers_.size(); ++i) {
      auto&& command_buffer = command_buffers_[i];
      vk::CommandBufferBeginInfo command_buffer_bi;
      command_buffer_bi.flags =
          vk::CommandBufferUsageFlagBits::eSimultaneousUse;
      command_buffer->begin(command_buffer_bi);

      vk::RenderPassBeginInfo render_pass_bi;
      render_pass_bi.renderPass = *render_pass_;
      render_pass_bi.renderArea.offset = vk::Offset2D{0, 0};
      render_pass_bi.renderArea.extent =
          vk::Extent2D{swap_chain->width(), swap_chain->height()};
      vk::ClearValue clear_value;
      clear_value.color = std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f};
      render_pass_bi.clearValueCount = 1;
      render_pass_bi.pClearValues = &clear_value;
      render_pass_bi.framebuffer = *frame_buffers_[i];

      command_buffer->beginRenderPass(render_pass_bi,
                                      vk::SubpassContents::eInline);
      command_buffer->bindPipeline(vk::PipelineBindPoint::eGraphics,
                                   *pipeline_);
      command_buffer->draw(3, 1, 0, 0);
      command_buffer->endRenderPass();
    }
  }

  void DrawFrame() {
    auto vk_device = tvk_device_->device();
    auto swap_chain = tvk_device_->main_swap_chain();
    auto tvk_swap_chain = static_cast<gfx::tvk::TvkSwapChain*>(swap_chain);
    auto image_index = tvk_swap_chain->AcquireNextImage(tvk_device_.get());

    auto& image = tvk_swap_chain->images()[image_index];
    vk::SubmitInfo submit_info;
    vk::Semaphore wait_semaphores[] = {image.acquire_image_semaphore.get()};
    vk::PipelineStageFlags wait_stages[] = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &(command_buffers_[image_index].get());
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &image.render_semaphore.get();
    auto& queue_table = tvk_device_->context().queue_table();
    auto& graphics_queue =
        queue_table.find(vk::QueueFlagBits::eGraphics)->second;
    graphics_queue.submit(1, &submit_info, vk::Fence());

    vk::PresentInfoKHR present_info;
    auto vk_swap_chain = tvk_swap_chain->swap_chain();
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &image.render_semaphore.get();
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &vk_swap_chain;
    present_info.pImageIndices = &image_index;

    graphics_queue.presentKHR(present_info);
  }

 private:
  std::shared_ptr<gfx::tvk::TvkDevice> tvk_device_;
  vk::UniqueShaderModule vs_module_;
  vk::UniqueShaderModule fs_module_;
  vk::UniquePipelineLayout pipeline_layout_;
  vk::UniqueRenderPass render_pass_;
  vk::UniquePipeline pipeline_;
  vk::UniqueCommandPool command_pool_;
  std::vector<vk::UniqueFramebuffer> frame_buffers_;
  std::vector<vk::UniqueCommandBuffer> command_buffers_;
};

int main(int argc, char* argv[]) {
  app::Application application;

  application.on_resize_window() = [](std::uint32_t w, std::uint32_t h) {
    TEMP_LOG_TRACE("On window resize. (", w, ", ", h, ")");
  };
  auto window = application.native_window_handle();
  auto window_size = app::GetWindowViewSize(window);
  auto gfx_device = gfx::CreateDevice(gfx::ApiType::kVulkan, window,
                                      window_size.width, window_size.height);

  TestRenderer renderer(gfx_device);

  application.on_update() = [&renderer]() { renderer.DrawFrame(); };

  application.Run();

  return 0;
}
