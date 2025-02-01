#pragma once

#include "../VulkanInclude.h"

namespace vul::Graphics {
	class RenderPass {
	public:
		RenderPass(VkFormat imageFormat, VkDevice logicalDevice);
		~RenderPass();

		inline VkRenderPass GetRaw() { return m_Raw; }

	private:
		VkRenderPass m_Raw;
		VkDevice m_LogicalDevice;
	};
}