#pragma once

#include <vul/core/AppSpecifications.h>

#include "../VulkanInclude.h"

namespace vul::Graphics {
	class VulkanInstance {
	public:
		VulkanInstance(const AppSpecifications& specs);
		~VulkanInstance();

		inline VkInstance GetRaw() { return m_Raw; }

	private:
		VkInstance m_Raw;
	};
}