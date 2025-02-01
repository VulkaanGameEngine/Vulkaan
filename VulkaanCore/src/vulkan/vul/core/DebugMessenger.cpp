#ifdef VUL_CONF_DEBUG
#include "DebugMessenger.h"

#include <vul/core/Log.h>

namespace vul {
	DebugMessenger::DebugMessenger(VkInstance vulkanInstance) {
		m_VulkanInstance = vulkanInstance;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;

		VUL_CORE_ASSERT(createDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr, &m_Raw) == VK_SUCCESS, "Failed to set up debug messenger!");
	}

	DebugMessenger::~DebugMessenger() {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanInstance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(m_VulkanInstance, m_Raw, nullptr);
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) VUL_LOG_CORE_ERROR("Vulkan's debugCallback: {}", pCallbackData->pMessage);
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) VUL_LOG_CORE_WARN("Vulkan's debugCallback: {}", pCallbackData->pMessage);
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) VUL_LOG_CORE_INFO("Vulkan's debugCallback: {}", pCallbackData->pMessage);
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) VUL_LOG_CORE_TRACE("Vulkan's debugCallback: {}", pCallbackData->pMessage);
		else VUL_LOG_CORE_WARN("Vulkan's debugCallback: Tried logging \"{}\", but messageSeverity wasn't high enough!", pCallbackData->pMessage);

		return VK_FALSE;
	}

	VkResult DebugMessenger::createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func == nullptr) return VK_ERROR_EXTENSION_NOT_PRESENT;
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
}
#endif