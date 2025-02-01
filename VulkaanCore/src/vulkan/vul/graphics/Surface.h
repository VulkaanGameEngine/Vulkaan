#pragma once

#include "../VulkanInclude.h"

namespace vul::Graphics {
	class Surface {
	public:
		Surface(VkInstance vulkanInstance, GLFWwindow* window);
		~Surface();

		inline VkSurfaceKHR GetRaw() { return m_Raw; }

	private:
		VkInstance m_VulkanInstance;

		VkSurfaceKHR m_Raw;
	};
}