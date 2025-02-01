#include "VulkanWindow.h"

#include "../VulkanInclude.h"

namespace vul::Graphics {
	VulkanWindow::VulkanWindow(glm::ivec2 size, const char* title) : Window() {
		m_Raw = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
	}

	VulkanWindow::~VulkanWindow() {
		glfwDestroyWindow(static_cast<GLFWwindow*>(m_Raw));
	}

	bool VulkanWindow::ShouldClose() const {
		return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Raw));
	}
}