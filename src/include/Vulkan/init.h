#pragma once

#include "../Vector.h"

#include <vulkan/vulkan.h>
#include <stdio.h>

VkInstance vk_create_instance(const char *name, const char *engine_name);
MVector<VkPhysicalDevice> vk_get_physical_devices(VkInstance instance);
VkDevice vk_create_logical_device(VkPhysicalDevice physical_device, Uint queue_family_idx);
VkRenderPass vk_create_render_pass(VkDevice device, VkFormat imageFormat);
VkPipeline vk_create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass renderPass);