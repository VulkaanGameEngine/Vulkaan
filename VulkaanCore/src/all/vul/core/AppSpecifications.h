#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../graphics/GPURequirements.h"

namespace vul {
	struct AppSpecifications {
		glm::ivec2 Size;
		glm::ivec3 Version;

		const char* Title;

		bool Resizable;

		Graphics::GPURequirements GPURequirements;

		glm::vec3 ClearColor;

		bool PreferPowerEfficiency;

		AppSpecifications() {
			Size = glm::ivec2{ 1280, 720 };
			Version = glm::ivec3{ 0, 0, 0 };
			Title = "Hello, Vulkaan!";
			Resizable = true;
			GPURequirements = {};
			ClearColor = glm::vec3{ 0, 0, 0 };
			PreferPowerEfficiency = false;
		}
	};
}