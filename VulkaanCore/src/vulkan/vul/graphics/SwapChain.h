#pragma once

#include "../VulkanInclude.h"

#include <vector>

namespace vul::Graphics {
	class SwapChain {
	public:
		SwapChain(VkPhysicalDevice device, VkDevice logicalDevice, VkSurfaceKHR surface, bool preferPowerEfficiency, GLFWwindow* window);
		~SwapChain();

		inline VkSwapchainKHR GetRaw() { return m_Raw; }

		inline VkFormat GetImageFormat() { return m_ImageFormat; }

	private:
		VkSwapchainKHR m_Raw;
		VkDevice m_LogicalDevice;

		std::vector<VkImage> m_Images;
		VkFormat m_ImageFormat;
		VkExtent2D m_Extent;
		std::vector<VkImageView> m_ImageViews;
	};
}