#pragma once

#include "../VulkanInclude.h"

#include <vector>

namespace vul::Graphics {
	class Framebuffers {
	public:
		Framebuffers();
		~Framebuffers();
		
		inline std::vector<VkFramebuffer> GetRaw() { return m_Raw; }

	private:
		std::vector<VkFramebuffer> m_Raw;
	};
}