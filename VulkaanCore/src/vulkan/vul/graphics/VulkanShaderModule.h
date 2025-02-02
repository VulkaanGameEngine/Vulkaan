#pragma once

#include <vul/graphics/ShaderModule.h>

#include "../VulkanInclude.h"

namespace vul::Graphics {
	class VulkanShaderModule : public ShaderModule {
	public:
		VulkanShaderModule(const char* name, ShaderType shaderType, std::vector<char> contents);
		virtual ~VulkanShaderModule() override;

		inline VkShaderModule GetRaw() { return m_Raw; }

		inline VkPipelineShaderStageCreateInfo GetStageCreateInfo() { return m_StageCreateInfo; }

	private:
		VkShaderModule m_Raw = nullptr;
		VkPipelineShaderStageCreateInfo m_StageCreateInfo{};
	};
}