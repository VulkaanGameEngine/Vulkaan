#pragma once

#include <vector>

namespace vul {
	class RequiredExtensions {
	public:
		static std::vector<const char*> Get();
	};
}