#pragma once

#include <vul/graphics/GPURequirements.h>

#include "../VulkanInclude.h"

#include <vector>

namespace vul::PhysicalDevice {
	VkPhysicalDevice Pick(VkInstance vulkanInstance, VkSurfaceKHR surface, const Graphics::GPURequirements& gpuRequirements);

	uint32_t RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface, const Graphics::GPURequirements& gpuRequirements);

	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	const std::vector<const char*> DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
}