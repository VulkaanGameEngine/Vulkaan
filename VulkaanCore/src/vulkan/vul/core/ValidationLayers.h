#pragma once

#include "../VulkanInclude.h"

#include <vector>

namespace vul::ValidationLayers {
#ifdef VUL_CONF_DEBUG
	const std::vector<const char*> Vector = {
		"VK_LAYER_KHRONOS_validation",
		//"VK_LAYER_LUNARG_api_dump"
	};

	bool CheckSupport();
#endif
}