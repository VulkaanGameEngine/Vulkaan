#pragma once

#include <glm/vec2.hpp>

#ifdef VUL_PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef VUL_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

#include <memory>

namespace vul::Graphics {
	class Window {
	public:
		Window(glm::ivec2 size, const char* title);
		~Window();

		inline GLFWwindow* GetRaw() const { return m_RawWindow; }

		bool ShouldClose() const;

	private:
		GLFWwindow* m_RawWindow;
	};
}