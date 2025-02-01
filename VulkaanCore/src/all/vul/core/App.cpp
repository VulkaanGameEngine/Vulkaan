#include "App.h"

namespace vul {
	App* App::s_Instance = nullptr;

	App::App(const AppSpecifications& specs) {
		s_Instance = this;

		m_Renderer = new Graphics::Renderer(specs.Size, specs.Title, specs.Resizable, specs.Version, specs.GPURequirements, specs.ClearColor, specs.PreferEnergyEfficiency);
	}

	App::~App() {
		delete m_Renderer;
	}

	int App::Run() {
		while (!m_Renderer->GetWindow()->ShouldClose()) {
			m_Renderer->PollEvents();
			m_Renderer->Draw();
		}

		m_Renderer->WaitIdle();

		return 0;
	}
}