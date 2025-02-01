#include "LogicalDevice.h"

#include "PhysicalDevice.h"
#include "QueueFamilyIndices.h"
#include "ValidationLayers.h"

#include <vul/core/Log.h>

#include <vector>
#include <set>

namespace vul {
	LogicalDevice::LogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
		QueueFamilyIndices indices = QueueFamilyIndices::Find(physicalDevice, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		float queuePriority = 1.0f;

		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(PhysicalDevice::DeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = PhysicalDevice::DeviceExtensions.data();

#ifdef VUL_CONF_DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers::Vector.size());
		createInfo.ppEnabledLayerNames = ValidationLayers::Vector.data();
#else
		createInfo.enabledLayerCount = 0;
#endif

		VUL_CORE_ASSERT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_Raw) == VK_SUCCESS, "Failed to create logical device!");

		vkGetDeviceQueue(m_Raw, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Raw, indices.PresentFamily.value(), 0, &m_PresentQueue);
	}

	LogicalDevice::~LogicalDevice() {
		vkDestroyDevice(m_Raw, nullptr);
	}
}