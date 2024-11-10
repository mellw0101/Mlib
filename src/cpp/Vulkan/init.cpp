#include "../../include/Vulkan/init.h"

MVector<char> vk_read_file(const char *path) {
  std::ifstream file(path, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    fprintf(stderr, "Failed to open file: %s\n", path);
    return {};
  }
  Ulong         size = file.tellg();
  MVector<char> buffer(size);
  file.seekg(0);
  file.read(buffer.data(), size);
  file.close();
  return buffer;
}

VkShaderModule vk_create_shader_module(VkDevice device, const MVector<char> &code) {
  if (code.empty()) {
    return NULL;
  }
  VkShaderModuleCreateInfo info {};
  info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  info.codeSize = code.size();
  info.pCode    = (const Uint *)code.data();
  VkShaderModule module;
  if (vkCreateShaderModule(device, &info, NULL, &module) != VK_SUCCESS) {
    fprintf(stderr, "Failed to create VkShaderModule.\n");
    return NULL;
  }
  return module;
}

VkInstance vk_create_instance(const char *name, const char *engine_name) {
  VkApplicationInfo app_info {};
  app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName   = name;
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName        = engine_name;
  app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion         = VK_API_VERSION_1_0;
  VkInstanceCreateInfo create_info {};
  create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  VkInstance instance;
  if (vkCreateInstance(&create_info, NULL, &instance) != VK_SUCCESS) {
    fprintf(stderr, "Failed to create VkInstance.\n");
    return NULL;
  }
  return instance;
}

MVector<VkPhysicalDevice> vk_get_physical_devices(VkInstance instance) {
  Uint device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, NULL);
  MVector<VkPhysicalDevice> result(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, result.data());
  return result;
}

VkDevice vk_create_logical_device(VkPhysicalDevice physical_device, Uint queue_family_idx) {
  float                   queue_priority = 1.0f;
  VkDeviceQueueCreateInfo queue_create_info {};
  queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_create_info.queueFamilyIndex = queue_family_idx;
  queue_create_info.queueCount       = 1;
  queue_create_info.pQueuePriorities = &queue_priority;
  VkDeviceCreateInfo create_info {};
  create_info.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pQueueCreateInfos    = &queue_create_info;
  create_info.queueCreateInfoCount = 1;
  VkDevice device;
  if (vkCreateDevice(physical_device, &create_info, NULL, &device) != VK_SUCCESS) {
    fprintf(stderr, "Failed to create VkDevice.\n");
    return NULL;
  }
  return device;
}

VkRenderPass vk_create_render_pass(VkDevice device, VkFormat imageFormat) {
  VkAttachmentDescription colorAttachment {};
  colorAttachment.format         = imageFormat;
  colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef {};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass {};
  subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments    = &colorAttachmentRef;

  VkRenderPassCreateInfo renderPassInfo {};
  renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments    = &colorAttachment;
  renderPassInfo.subpassCount    = 1;
  renderPassInfo.pSubpasses      = &subpass;

  VkRenderPass renderPass;
  if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
    fprintf(stderr, "Failed to create render pass.\n");
    return nullptr;
  }

  return renderPass;
}

VkPipeline vk_create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass renderPass) {
  // Load shader code.
  MVector<char> vertShaderCode = vk_read_file("vert.spv");
  MVector<char> fragShaderCode = vk_read_file("frag.spv");

  VkShaderModule vertShaderModule = vk_create_shader_module(device, vertShaderCode);
  VkShaderModule fragShaderModule = vk_create_shader_module(device, fragShaderCode);

  // Define shader stages
  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertShaderModule;
  vertShaderStageInfo.pName  = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName  = "main";

  VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount   = 0;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport{};
  viewport.x        = 0.0f;
  viewport.y        = 0.0f;
  viewport.width    = (float)extent.width;
  viewport.height   = (float)extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports    = &viewport;
  viewportState.scissorCount  = 1;
  viewportState.pScissors     = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable        = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth               = 1.0f;
  rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments    = &colorBlendAttachment;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

  VkPipelineLayout pipelineLayout;
  if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    fprintf(stderr, "Failed to create pipeline layout.\n");
    return nullptr;
  }

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount          = 2;
  pipelineInfo.pStages             = shaderStages;
  pipelineInfo.pVertexInputState   = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState      = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState   = &multisampling;
  pipelineInfo.pColorBlendState    = &colorBlending;
  pipelineInfo.layout              = pipelineLayout;
  pipelineInfo.renderPass          = renderPass;
  pipelineInfo.subpass             = 0;

  VkPipeline graphicsPipeline;
  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
    fprintf(stderr, "Failed to create graphics pipeline.\n");
    return nullptr;
  }

  // Cleanup shader modules after pipeline creation
  vkDestroyShaderModule(device, vertShaderModule, nullptr);
  vkDestroyShaderModule(device, fragShaderModule, nullptr);

  return graphicsPipeline;
}