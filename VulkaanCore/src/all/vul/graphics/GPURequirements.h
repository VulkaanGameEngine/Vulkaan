#pragma once

namespace vul::Graphics {
	struct SupportsShadersRequirements {
		bool GeometryShader;

		SupportsShadersRequirements() {
			GeometryShader = true;
		}
	};

	struct GPURequirements {
		SupportsShadersRequirements SupportsShaders;
		bool Discrete;
		uint32_t MaxTextureSize;

		GPURequirements() {
			SupportsShaders = SupportsShadersRequirements();
			Discrete = true;
			MaxTextureSize = 0;
		}
	};
}