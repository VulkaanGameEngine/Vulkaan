#pragma once

#include <vul/graphics/Window.h>

#include <glm/vec2.hpp>

namespace vul::Graphics {
	class VulkanWindow : public Window {
	public:
		VulkanWindow(glm::ivec2 size, const char* title);
		~VulkanWindow();

		virtual bool ShouldClose() const override;
	};
}