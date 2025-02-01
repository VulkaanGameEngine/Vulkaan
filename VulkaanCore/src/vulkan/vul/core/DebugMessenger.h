#pragma once

#include "../VulkanInclude.h"

namespace vul {
	class DebugMessenger {
	public:
		DebugMessenger(VkInstance vulkanInstance);
		~DebugMessenger();

	private:
		VkInstance m_VulkanInstance;

		VkDebugUtilsMessengerEXT m_Raw;

		VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);
}