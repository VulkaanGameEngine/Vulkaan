#pragma once

#include "../VulkanInclude.h"

namespace vul::Graphics {
	class CommandPool {
	public:
		CommandPool(VkPhysicalDevice physicalDevice);
		~CommandPool();

		inline VkCommandPool GetRaw() { return m_Raw; }

	private:
		VkCommandPool m_Raw;
	};
}