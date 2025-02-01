#include "VulkanRenderer.h"

#include "VulkanWindow.h"

#include "../core/PhysicalDevice.h"

#include "../VulkanInclude.h"

#include <vul/core/Log.h>

namespace vul::Graphics {
	VulkanRenderer::VulkanRenderer(const AppSpecifications& specs) : Renderer() {
		VUL_CORE_ASSERT(glfwInit() == GLFW_TRUE, "Failed to initialize GLFW!");

		m_VulkanInstance = new VulkanInstance(specs);
		m_Window = new VulkanWindow(specs.Size, specs.Title);

#ifdef VUL_CONF_DEBUG
		m_DebugMessenger = new DebugMessenger(m_VulkanInstance->GetRaw());
#endif
		m_Surface = new Surface(m_VulkanInstance->GetRaw(), static_cast<GLFWwindow*>(m_Window->GetRaw()));

		VkPhysicalDevice physicalDevice = PhysicalDevice::Pick(m_VulkanInstance->GetRaw(), m_Surface->GetRaw(), specs.GPURequirements);

		m_LogicalDevice = new LogicalDevice(physicalDevice, m_Surface->GetRaw());

		m_SwapChain = new SwapChain(physicalDevice, m_LogicalDevice->GetRaw(), m_Surface->GetRaw(), specs.PreferPowerEfficiency, static_cast<GLFWwindow*>(m_Window->GetRaw()));

		m_RenderPass = new RenderPass(m_SwapChain->GetImageFormat(), m_LogicalDevice->GetRaw());

		m_GraphicsPipeline = nullptr;
	}

	VulkanRenderer::~VulkanRenderer() {
		delete m_GraphicsPipeline;
		delete m_RenderPass;
		delete m_SwapChain;
		delete m_LogicalDevice;
#ifdef VUL_CONF_DEBUG
		delete m_DebugMessenger;
#endif
		delete m_Surface;
		delete m_VulkanInstance;
		delete m_Window;

		glfwTerminate();
	}

	void VulkanRenderer::PollEvents() const {
		glfwPollEvents();
	}
	
	void VulkanRenderer::Draw() const {

	}

	void VulkanRenderer::WaitUntilIdle() const {
		vkDeviceWaitIdle(m_LogicalDevice->GetRaw());
	}

	void VulkanRenderer::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) {

	}
}