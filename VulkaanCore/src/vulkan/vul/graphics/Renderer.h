#pragma once

#include <glm/vec3.hpp>

#include "Window.h"

#include "../utils/VulkanUtils.h"

#include <vul/filesystem/File.h>

namespace vul::Graphics {
	class Renderer {
	public:
		Renderer(glm::ivec2 size, const char* title, bool resizable, glm::ivec3 appversion, const Graphics::GPURequirements& gpuRequirements, glm::vec3 clearColor, bool preferEnergyEfficiency);
		~Renderer();

		static Renderer* GetInstance() { return s_Instance; }

		Window* GetWindow() const { return m_Window; }

		void PollEvents();

		void Draw();

		void WaitIdle();

	private:
		void createVulkanInstance(glm::ivec2 size, const char* title, bool resizable, glm::ivec3 appversion);

		static Renderer* s_Instance;

		Window* m_Window;

		VkInstance m_VulkanInstance;

#ifdef VUL_CONF_DEBUG
		void setupDebugMessenger();
		VkDebugUtilsMessengerEXT m_DebugMessenger;
#endif

		void pickPhysicalDevice(const Graphics::GPURequirements& gpuRequirements);
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;

		void createLogicalDevice();
		VkDevice m_LogicalDevice;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		void createSurface();
		VkSurfaceKHR m_Surface;

		void createSwapChain(bool preferEnergyEfficiency);
		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		void createImageViews();
		std::vector<VkImageView> m_SwapChainImageViews;

		void createRenderPass();
		VkRenderPass m_RenderPass;

		void createGraphicsPipeline();
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;

		VkShaderModule createShaderModule(const std::vector<char>& code);

		void createFramebuffers();
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;
		
		void createCommandPool();
		VkCommandPool m_CommandPool;

		void createCommandBuffer();
		VkCommandBuffer m_CommandBuffer;

		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		VkClearValue m_ClearColor;

		void createSyncObjects();
		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinishedSemaphore;
		VkFence m_InFlightFence;
	};
}