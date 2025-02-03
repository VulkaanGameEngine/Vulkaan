#include "Framebuffers.h"

#include "VulkanRenderer.h"

#include "../core/LogicalDevice.h"

#include <vul/core/App.h>
#include <vul/core/Log.h>

namespace vul::Graphics {
	Framebuffers::Framebuffers() {
		VulkanRenderer* renderer = static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer());
		
		std::vector<VkImageView> imageViews = renderer->GetSwapChain()->GetImageViews();
		VkRenderPass renderPass = renderer->GetRenderPass()->GetRaw();
		VkExtent2D extent = renderer->GetSwapChain()->GetExtent();
		VkDevice logicalDevice = renderer->GetLogicalDevice()->GetRaw();

		m_Raw.resize(imageViews.size());

		for (size_t i = 0; i < imageViews.size(); i++) {
			VkImageView attachments[] = {
				imageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = extent.width;
			framebufferInfo.height = extent.height;
			framebufferInfo.layers = 1;

			VUL_CORE_ASSERT(vkCreateFramebuffer(logicalDevice, &framebufferInfo, nullptr, &m_Raw[i]) == VK_SUCCESS, "Failed to create framebuffer!");
		}
	}

	Framebuffers::~Framebuffers() {
		VkDevice logicalDevice = static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer())->GetLogicalDevice()->GetRaw();
		
		for (VkFramebuffer framebuffer : m_Raw) {
			vkDestroyFramebuffer(logicalDevice, framebuffer, nullptr);
		}
	}
}