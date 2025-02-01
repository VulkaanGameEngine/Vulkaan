#include "VulkanInstance.h"

#include "../core/RequiredExtensions.h"
#include "../core/ValidationLayers.h"

#include <vul/core/Log.h>

#include <vul_defines.h>

namespace vul::Graphics {
	VulkanInstance::VulkanInstance(const AppSpecifications& specs) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		if (!specs.Resizable) glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef VUL_CONF_DEBUG
		VUL_CORE_ASSERT(ValidationLayers::CheckSupport(), "Validation layers requested, but not available!");
#endif

		uint32_t vulkanExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr);
		std::vector<VkExtensionProperties> vulkanExtensions(vulkanExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, vulkanExtensions.data());
		for (const VkExtensionProperties& vulkanExtension : vulkanExtensions) VUL_LOG_CORE_TRACE("Available Vulkan extension: {}", vulkanExtension.extensionName);

		std::vector<const char*> glfwExtensions = RequiredExtensions::Get();
		for (const char* glfwExtension : glfwExtensions) VUL_LOG_CORE_TRACE("Required GLFW extension: {}", glfwExtension);

		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = specs.Title;
		appInfo.pNext = nullptr;
		appInfo.applicationVersion = VK_MAKE_VERSION(specs.Version.x, specs.Version.y, specs.Version.z);
		appInfo.pEngineName = VUL_ENGINE_NAME;
		appInfo.engineVersion = VK_MAKE_VERSION(VUL_ENGINE_VERSION_MAJOR, VUL_ENGINE_VERSION_MINOR, VUL_ENGINE_VERSION_PATCH);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
#ifdef VUL_CONF_DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers::Vector.size());
		createInfo.ppEnabledLayerNames = ValidationLayers::Vector.data();
#else
		createInfo.enabledLayerCount = 0;
#endif
		createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
		createInfo.ppEnabledExtensionNames = glfwExtensions.data();

		VUL_CORE_ASSERT(vkCreateInstance(&createInfo, nullptr, &m_Raw) == VK_SUCCESS, "Failed to initialize Vulkan!");
	}

	VulkanInstance::~VulkanInstance() {
		vkDestroyInstance(m_Raw, nullptr);
	}
}