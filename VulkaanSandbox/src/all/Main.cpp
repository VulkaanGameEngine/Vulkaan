#include <vul/Vulkaan.h>

class SandboxApp : public vul::App {
public:
	SandboxApp(const vul::AppSpecifications& specs) : vul::App(specs) {
		vul::Graphics::ShaderModule* vertexShader = vul::Graphics::ShaderLibrary::Load("triangleVert", vul::Graphics::ShaderType::Vertex, "Assets/Shaders/triangleShader-vert.spv");
		vul::Graphics::ShaderModule* fragmentShader = vul::Graphics::ShaderLibrary::Load("triangleFrag", vul::Graphics::ShaderType::Fragment, "Assets/Shaders/triangleShader-frag.spv");

		GetRenderer()->SetGraphicsPipeline(vul::Graphics::CreateGraphicsPipeline(vertexShader, fragmentShader));
	}

	~SandboxApp() {

	}
};

vul::App* vul::CreateApplication() {
	vul::AppSpecifications specs;
	
	specs.Version = glm::ivec3{ 1, 0, 0 };
	specs.Resizable = false;

	specs.GPURequirements.Discrete = false;

	return new SandboxApp(specs);
}