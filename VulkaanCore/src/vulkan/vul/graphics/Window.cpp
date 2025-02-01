#include "Window.h"

#include <vul/core/Log.h>

namespace vul::Graphics {
	Window::Window(glm::ivec2 size, const char* title) {
		m_RawWindow = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
	}

	Window::~Window() {
		VUL_LOG_CORE_TRACE("Window::~Window()");
		glfwDestroyWindow(m_RawWindow);
	}

	bool Window::ShouldClose() const {
		return glfwWindowShouldClose(m_RawWindow);
	}
}