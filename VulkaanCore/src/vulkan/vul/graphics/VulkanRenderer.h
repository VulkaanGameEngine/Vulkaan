#pragma once

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

		inline LogicalDevice* GetLogicalDevice() { return m_LogicalDevice; }
		inline RenderPass* GetRenderPass() { return m_RenderPass; }

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
	};
}