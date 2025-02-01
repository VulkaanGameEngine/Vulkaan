#pragma once

#include <vul/graphics/GraphicsPipeline.h>

#include "../VulkanInclude.h"

namespace vul::Graphics {
	class VulkanGraphicsPipeline : public GraphicsPipeline {
	public:
		VulkanGraphicsPipeline(ShaderModule* vertexShaderModule, ShaderModule* fragmentShaderModule);
		virtual ~VulkanGraphicsPipeline() override;

		inline VkPipeline GetRaw() { return m_Raw; }

	private:
		VkPipeline m_Raw;
		VkPipelineLayout m_Layout;
	};
}