#pragma once

#include "Window.h"

#include "GraphicsPipeline.h"

namespace vul::Graphics {
	class Renderer {
	public:
		Renderer();
		~Renderer();

		inline static Renderer* GetInstance() { return s_Instance; }

		inline Window* GetWindow() const { return m_Window; }

		virtual void PollEvents() const = 0;
		virtual void Draw() const = 0;
		virtual void WaitUntilIdle() const = 0;

		virtual void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) = 0;

	protected:
		static Renderer* s_Instance;

		Window* m_Window = nullptr;
	};
}