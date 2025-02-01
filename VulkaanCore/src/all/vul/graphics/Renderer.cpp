#include "Renderer.h"

namespace vul::Graphics {
	Renderer* Renderer::s_Instance = nullptr;

	Renderer::Renderer() {
		s_Instance = this;
	}

	Renderer::~Renderer() {

	}
}