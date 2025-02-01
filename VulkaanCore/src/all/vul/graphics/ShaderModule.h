#pragma once

#include <vector>
#include <map>

namespace vul::Graphics {
	enum class ShaderType {
		Unknown = 0,
		Vertex, Fragment
	};

	class ShaderModule {
	public:
		ShaderModule(const char* name);
		virtual ~ShaderModule() = default;

		inline const char* GetName() { return m_Name; }

	private:
		const char* m_Name;
	};

	class ShaderLibrary {
	public:
		static ShaderModule* Load(const char* name, ShaderType shaderType, const char* filename);
		static void Unload(ShaderModule* shader);

	private:
		static std::multimap<const char*, ShaderModule*> s_LoadedModules;
	};
}