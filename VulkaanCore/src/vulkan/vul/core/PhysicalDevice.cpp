#include "PhysicalDevice.h"

#include "QueueFamilyIndices.h"

#include "../graphics/SwapChainSupportDetails.h"

#include <vul/core/Log.h>

#include <set>

namespace vul::PhysicalDevice {
	VkPhysicalDevice Pick(VkInstance vulkanInstance, VkSurfaceKHR surface, const Graphics::GPURequirements& gpuRequirements) {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
		if (deviceCount == 0) VUL_LOG_CORE_CRITICAL("Failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

		std::multimap<uint32_t, VkPhysicalDevice> candidates;

		for (const VkPhysicalDevice& device : devices) {
			if (IsDeviceSuitable(device, surface)) candidates.insert(std::make_pair(RateDeviceSuitability(device, surface, gpuRequirements), device));
		}

		if (candidates.rbegin()->first > 0) return candidates.rbegin()->second;
		
		VUL_LOG_CORE_CRITICAL("Failed to find a suitable GPU!");
		return nullptr;
	}

	uint32_t RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface, const Graphics::GPURequirements& gpuRequirements) {
		QueueFamilyIndices indices = QueueFamilyIndices::Find(device, surface);

		if (!indices.IsComplete()) return 0;

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		return (
			gpuRequirements.SupportsShaders.GeometryShader && !deviceFeatures.geometryShader ||
			gpuRequirements.Discrete && deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
			gpuRequirements.MaxTextureSize && deviceProperties.limits.maxImageDimension2D < gpuRequirements.MaxTextureSize
		) ? 0 : deviceProperties.limits.maxImageDimension2D;
	}

	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
		QueueFamilyIndices indices = QueueFamilyIndices::Find(device, surface);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			Graphics::SwapChainSupportDetails swapChainSupport = Graphics::SwapChainSupportDetails::Query(device, surface);
			swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
		}

		return indices.IsComplete() && extensionsSupported && swapChainAdequate;
	}

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());

		for (const VkExtensionProperties& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}
}