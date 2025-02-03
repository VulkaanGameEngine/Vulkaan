#pragma once

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Framebuffers.h"
#include "RenderPass.h"
#include "Surface.h"
#include "SwapChain.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanInstance.h"

#ifdef VUL_CONF_DEBUG
#include "../core/DebugMessenger.h"
#endif

#include "../core/LogicalDevice.h"

#include <vul/graphics/Renderer.h>

#include <vul/core/AppSpecifications.h>

namespace vul::Graphics {
	class VulkanRenderer : public Renderer {
	public:
		VulkanRenderer(const AppSpecifications& specs);
		~VulkanRenderer();

		virtual void PollEvents() const override;
		virtual void Draw() const override;
		virtual void WaitUntilIdle() const override;

		virtual void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) override;

		inline Surface* GetSurface() { return m_Surface; }
		inline LogicalDevice* GetLogicalDevice() { return m_LogicalDevice; }
		inline SwapChain* GetSwapChain() { return m_SwapChain; }
		inline RenderPass* GetRenderPass() { return m_RenderPass; }
		inline CommandPool* GetCommandPool() { return m_CommandPool; }

	private:
		VulkanInstance* m_VulkanInstance;

#ifdef VUL_CONF_DEBUG
		DebugMessenger* m_DebugMessenger;
#endif

		Surface* m_Surface;
		LogicalDevice* m_LogicalDevice;
		SwapChain* m_SwapChain;
		RenderPass* m_RenderPass;
		VulkanGraphicsPipeline* m_GraphicsPipeline;
		Framebuffers* m_Framebuffers;
		CommandPool* m_CommandPool;
		CommandBuffer* m_CommandBuffer;
	};
}