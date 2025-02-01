#include "VulkanGraphicsPipeline.h"

#include "VulkanRenderer.h"
#include "VulkanShaderModule.h"

#include <vul/core/App.h>
#include <vul/core/Log.h>

namespace vul::Graphics {
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(ShaderModule* vertexShaderModule, ShaderModule* fragmentShaderModule) {
		VulkanShaderModule* vulkanVertexShaderModule = static_cast<VulkanShaderModule*>(vertexShaderModule);
		VulkanShaderModule* vulkanFragmentShaderModule = static_cast<VulkanShaderModule*>(fragmentShaderModule);

		VulkanRenderer* renderer = static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer());

		VkPipelineShaderStageCreateInfo shaderStages[] = { vulkanVertexShaderModule->GetStageCreateInfo(), vulkanFragmentShaderModule->GetStageCreateInfo() };

		/*VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		VUL_CORE_ASSERT(vkCreatePipelineLayout(renderer->GetLogicalDevice()->GetRaw(), &pipelineLayoutInfo, nullptr, &m_Layout) == VK_SUCCESS, "Failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_Layout;
		pipelineInfo.renderPass = renderer->GetRenderPass()->GetRaw();
		pipelineInfo.subpass = 0;

		VUL_CORE_ASSERT(vkCreateGraphicsPipelines(renderer->GetLogicalDevice()->GetRaw(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Raw) == VK_SUCCESS, "Failed to create graphics pipeline!");*/

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		VUL_CORE_ASSERT(vkCreatePipelineLayout(renderer->GetLogicalDevice()->GetRaw(), &pipelineLayoutInfo, nullptr, &m_Layout) == VK_SUCCESS, "Failed to create pipeline layout!");

		VkPipelineShaderStageCreateInfo vert = vulkanVertexShaderModule->GetStageCreateInfo();

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 1;
		pipelineInfo.pStages = &vert;  // Only a vertex shader
		pipelineInfo.layout = m_Layout;
		pipelineInfo.renderPass = renderer->GetRenderPass()->GetRaw();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		VUL_CORE_ASSERT(vkCreateGraphicsPipelines(renderer->GetLogicalDevice()->GetRaw(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Raw) == VK_SUCCESS, "Failed to create graphics pipeline!");

		delete vulkanFragmentShaderModule;
		delete vulkanVertexShaderModule;
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
		VulkanRenderer* renderer = static_cast<VulkanRenderer*>(App::GetInstance()->GetRenderer());

		vkDestroyPipeline(renderer->GetLogicalDevice()->GetRaw(), m_Raw, nullptr);
		vkDestroyPipelineLayout(renderer->GetLogicalDevice()->GetRaw(), m_Layout, nullptr);
	}
}