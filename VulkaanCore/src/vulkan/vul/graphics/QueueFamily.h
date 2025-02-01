#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>

namespace vul::Graphics {
	struct QueueFamilyIndices {
		std::optional<uint32_t> GraphicsFamily, PresentFamily;

		bool IsComplete() {
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
}