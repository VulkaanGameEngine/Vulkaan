#include "GraphicsPipeline.h"

#ifdef VUL_API_VULKAN
#include <vul/graphics/VulkanGraphicsPipeline.h>
#endif

namespace vul::Graphics {
	GraphicsPipeline::GraphicsPipeline() {

	}

#ifdef VUL_API_VULKAN
	GraphicsPipeline* CreateGraphicsPipeline(ShaderModule* vertexShaderModule, ShaderModule* fragmentShaderModule) {
		return new VulkanGraphicsPipeline(vertexShaderModule, fragmentShaderModule);
	}
#endif
}