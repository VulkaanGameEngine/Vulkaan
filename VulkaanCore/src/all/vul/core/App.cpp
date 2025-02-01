#include "App.h"

namespace vul {
	App* App::s_Instance = nullptr;

	App::App(const AppSpecifications& specs) {
		s_Instance = this;

#ifdef VUL_API_VULKAN
		m_Renderer = new Graphics::VulkanRenderer(specs);
#endif
	}

	App::~App() {
		delete m_Renderer;
	}

	int App::Run() {
		while (!m_Renderer->GetWindow()->ShouldClose()) {
			m_Renderer->PollEvents();
			m_Renderer->Draw();
		}

		m_Renderer->WaitUntilIdle();

		return 0;
	}
}