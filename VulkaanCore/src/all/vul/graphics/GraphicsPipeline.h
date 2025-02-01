#pragma once

#include <vul/graphics/ShaderModule.h>

namespace vul::Graphics {
	class GraphicsPipeline {
	public:
		GraphicsPipeline();
		virtual ~GraphicsPipeline() = default;
	};

	GraphicsPipeline* CreateGraphicsPipeline(ShaderModule* vertexShaderModule, ShaderModule* fragmentShaderModule);
}