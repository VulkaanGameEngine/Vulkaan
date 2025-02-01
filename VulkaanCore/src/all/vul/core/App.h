#pragma once

#include <memory>

#include <vul/graphics/Renderer.h>

#include "AppSpecifications.h"

namespace vul {
	class App {
	public:
		App(const AppSpecifications& specs);
		~App();

		static App* GetInstance() { return s_Instance; }

		Graphics::Renderer* GetRenderer() const { return m_Renderer; }

		int Run();

	private:
		static App* s_Instance;

		Graphics::Renderer* m_Renderer;
	};

	App* CreateApplication();
}