#include "SwapChain.h"

#include "SwapChainSupportDetails.h"

#include "../core/QueueFamilyIndices.h"

#include <vul/core/Log.h>

namespace vul::Graphics {
	SwapChain::SwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, bool preferPowerEfficiency, GLFWwindow* window) {
		m_LogicalDevice = logicalDevice;

		SwapChainSupportDetails swapChainSupport = SwapChainSupportDetails::Query(physicalDevice, surface);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes, preferPowerEfficiency);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities, window);

		uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;

		if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) imageCount = swapChainSupport.Capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = QueueFamilyIndices::Find(physicalDevice, surface);

		if (indices.GraphicsFamily == indices.PresentFamily) {
			VUL_LOG_CORE_TRACE("indices.GraphicsFamily == indices.PresentFamily, using VK_SHARING_MODE_EXCLUSIVE sharing mode!");
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		else {
			VUL_LOG_CORE_TRACE("indices.GraphicsFamily != indices.PresentFamily, using VK_SHARING_MODE_CONCURRENT sharing mode!");
			uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}

		createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_LogicalDevice, &createInfo, nullptr, &m_Raw) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create swap chain!");

		vkGetSwapchainImagesKHR(m_LogicalDevice, m_Raw, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(m_LogicalDevice, m_Raw, &imageCount, m_Images.data());

		m_ImageFormat = surfaceFormat.format;
		m_Extent = extent;

		// Create image views
		m_ImageViews.resize(m_Images.size());

		for (size_t i = 0; i < m_Images.size(); i++) {
			VkImageViewCreateInfo ivCreateInfo{};
			ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ivCreateInfo.image = m_Images[i];
			ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ivCreateInfo.format = m_ImageFormat;
			ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			ivCreateInfo.subresourceRange.baseMipLevel = 0;
			ivCreateInfo.subresourceRange.levelCount = 1;
			ivCreateInfo.subresourceRange.baseArrayLayer = 0;
			ivCreateInfo.subresourceRange.layerCount = 1;

			VUL_CORE_ASSERT(vkCreateImageView(m_LogicalDevice, &ivCreateInfo, nullptr, &m_ImageViews[i]) == VK_SUCCESS, "Failed to create image view!");
		}
	}

	SwapChain::~SwapChain() {
		for (auto imageView : m_ImageViews) {
			vkDestroyImageView(m_LogicalDevice, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_LogicalDevice, m_Raw, nullptr);
	}
}