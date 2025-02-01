#pragma once

#include "../VulkanInclude.h"

#include <optional>

namespace vul {
	struct QueueFamilyIndices {
		std::optional<uint32_t> GraphicsFamily, PresentFamily;

		inline bool IsComplete() { return GraphicsFamily.has_value() && PresentFamily.has_value(); }

		static QueueFamilyIndices Find(VkPhysicalDevice device, VkSurfaceKHR surface);
	};
}