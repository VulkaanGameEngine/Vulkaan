#pragma once

#include "../VulkanInclude.h"

namespace vul::Graphics {
	class CommandBuffer {
	public:
		CommandBuffer();
		~CommandBuffer();

		inline VkCommandBuffer GetRaw() { return m_Raw; }

	private:
		VkCommandBuffer m_Raw;
	};
}