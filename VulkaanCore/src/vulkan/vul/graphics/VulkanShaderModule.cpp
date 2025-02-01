#include "VulkanShaderModule.h"

#include <vul/core/App.h>
#include <vul/core/Log.h>

namespace vul::Graphics {
	VulkanShaderModule::VulkanShaderModule(const char* name, ShaderType shaderType, std::vector<char> contents) : ShaderModule(name) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = contents.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(contents.data());

		VUL_CORE_ASSERT(vkCreateShaderModule(static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer())->GetLogicalDevice()->GetRaw(), &createInfo, nullptr, &m_Raw) == VK_SUCCESS, "Failed to create shader module!");

		m_StageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		switch (shaderType) {
		case ShaderType::Vertex:
			m_StageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			break;
		case ShaderType::Fragment:
			m_StageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			break;
		default:
			break;
		}
		
		m_StageCreateInfo.module = m_Raw;
		m_StageCreateInfo.pName = "main";
	}

	VulkanShaderModule::~VulkanShaderModule() {
		vkDestroyShaderModule(static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer())->GetLogicalDevice()->GetRaw(), m_Raw, nullptr);
	}
}