#include <vul/Vulkaan.h>

class SandboxApp : public vul::App {
public:
	SandboxApp(const vul::AppSpecifications& specs) : vul::App(specs) {

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