#include "CommandPool.h"

#include "VulkanRenderer.h"

#include "../core/QueueFamilyIndices.h"

#include <vul/core/App.h>
#include <vul/core/Log.h>

namespace vul::Graphics {
	CommandPool::CommandPool(VkPhysicalDevice physicalDevice) {
		VulkanRenderer* renderer = static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer());

		QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::Find(physicalDevice, renderer->GetSurface()->GetRaw());

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

		if (vkCreateCommandPool(renderer->GetLogicalDevice()->GetRaw(), &poolInfo, nullptr, &m_Raw) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create command pool!");
	}

	CommandPool::~CommandPool() {
		vkDestroyCommandPool(static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer())->GetLogicalDevice()->GetRaw(), m_Raw, nullptr);
	}
}