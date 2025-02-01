#pragma once

#include "../VulkanInclude.h"

namespace vul {
	class LogicalDevice {
	public:
		LogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
		~LogicalDevice();

		inline VkDevice GetRaw() { return m_Raw; }
		//inline VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		//inline VkQueue GetPresentQueue() { return m_PresentQueue; }

	private:
		VkDevice m_Raw;

		VkQueue m_GraphicsQueue, m_PresentQueue;
	};
}