#include "Surface.h"

#include <vul/core/Log.h>

namespace vul::Graphics {
	Surface::Surface(VkInstance vulkanInstance, GLFWwindow* window) {
		m_VulkanInstance = vulkanInstance;

		VUL_CORE_ASSERT(glfwCreateWindowSurface(m_VulkanInstance, window, nullptr, &m_Raw) == VK_SUCCESS, "Failed to create surface!");
	}

	Surface::~Surface() {
		vkDestroySurfaceKHR(m_VulkanInstance, m_Raw, nullptr);
	}
}