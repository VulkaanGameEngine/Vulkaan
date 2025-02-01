#include "ShaderModule.h"

#include "../filesystem/File.h"

#ifdef VUL_API_VULKAN
#include <vul/graphics/VulkanShaderModule.h>
#endif

namespace vul::Graphics {
	std::multimap<const char*, ShaderModule*> ShaderLibrary::s_LoadedModules = std::multimap<const char*, ShaderModule*>();

	ShaderModule::ShaderModule(const char* name) {
		m_Name = name;
	}

	ShaderModule* ShaderLibrary::Load(const char* name, ShaderType shaderType, const char* filename) {
		if (s_LoadedModules.contains(name)) return s_LoadedModules.find(name)->second;

		Filesystem::File* file = new Filesystem::File(filename);
		std::vector<char> contents = file->Read();
		delete file;

#ifdef VUL_API_VULKAN
		ShaderModule* shader = new VulkanShaderModule(name, shaderType, contents);
#endif

		s_LoadedModules.insert(std::make_pair(name, shader));

		return shader;
	}

	void ShaderLibrary::Unload(ShaderModule* shader) {
		s_LoadedModules.erase(shader->GetName());
		delete shader;
	}
}