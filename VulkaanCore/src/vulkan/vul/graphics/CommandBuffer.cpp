#include "CommandBuffer.h"

#include "VulkanRenderer.h"

#include <vul/core/App.h>
#include <vul/core/Log.h>

namespace vul::Graphics {
	CommandBuffer::CommandBuffer() {
		VulkanRenderer* renderer = static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = renderer->GetCommandPool()->GetRaw();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VUL_CORE_ASSERT(vkAllocateCommandBuffers(renderer->GetLogicalDevice()->GetRaw(), &allocInfo, &m_Raw) == VK_SUCCESS, "Failed to allocate command buffers!");
	}

	CommandBuffer::~CommandBuffer() {
		VulkanRenderer* renderer = static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer());

		vkFreeCommandBuffers(renderer->GetLogicalDevice()->GetRaw(), renderer->GetCommandPool()->GetRaw(), 1, &m_Raw);
	}
}