#include "Renderer.h"

#include "vul_defines.h"

#include <vul/core/Log.h>

#include <map>
#include <set>

namespace vul::Graphics {
	Renderer* Renderer::s_Instance = nullptr;

	Renderer::Renderer(glm::ivec2 size, const char* title, bool resizable, glm::ivec3 appversion, const Graphics::GPURequirements& gpuRequirements, glm::vec3 clearColor, bool preferEnergyEfficiency) {
		s_Instance = this;

		createVulkanInstance(size, title, resizable, appversion);
		m_Window = new Window(size, title);
#ifdef VUL_CONF_DEBUG
		setupDebugMessenger();
#endif
		createSurface();
		pickPhysicalDevice(gpuRequirements);
		createLogicalDevice();
		createSwapChain(preferEnergyEfficiency);
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createCommandBuffer();
		createSyncObjects();

		m_ClearColor = { {{clearColor.r, clearColor.g, clearColor.b, 1.0f}} };
	}

	Renderer::~Renderer() {
		VUL_LOG_CORE_TRACE("Renderer::~Renderer()");

		vkDestroySemaphore(m_LogicalDevice, m_ImageAvailableSemaphore, nullptr);
		vkDestroySemaphore(m_LogicalDevice, m_RenderFinishedSemaphore, nullptr);
		vkDestroyFence(m_LogicalDevice, m_InFlightFence, nullptr);

		vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);

		for (VkFramebuffer framebuffer : m_SwapChainFramebuffers) {
			vkDestroyFramebuffer(m_LogicalDevice, framebuffer, nullptr);
		}

		vkDestroyPipeline(m_LogicalDevice, m_GraphicsPipeline, nullptr);

		vkDestroyPipelineLayout(m_LogicalDevice, m_PipelineLayout, nullptr);

		vkDestroyRenderPass(m_LogicalDevice, m_RenderPass, nullptr);

		for (auto imageView : m_SwapChainImageViews) vkDestroyImageView(m_LogicalDevice, imageView, nullptr);

		vkDestroySwapchainKHR(m_LogicalDevice, m_SwapChain, nullptr);

		vkDestroyDevice(m_LogicalDevice, nullptr);

#ifdef VUL_CONF_DEBUG
		Utils::DestroyDebugUtilsMessengerEXT(m_VulkanInstance, m_DebugMessenger, nullptr);
#endif

		vkDestroySurfaceKHR(m_VulkanInstance, m_Surface, nullptr);

		vkDestroyInstance(m_VulkanInstance, nullptr);

		delete m_Window;

		glfwTerminate();
	}

	void Renderer::PollEvents() {
		glfwPollEvents();
	}

	void Renderer::Draw() {
		vkWaitForFences(m_LogicalDevice, 1, &m_InFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(m_LogicalDevice, 1, &m_InFlightFence);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(m_LogicalDevice, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		vkResetCommandBuffer(m_CommandBuffer, 0);
		recordCommandBuffer(m_CommandBuffer, imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFence) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		
		VkSwapchainKHR swapChains[] = { m_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(m_PresentQueue, &presentInfo);
	}

	void Renderer::WaitIdle() {
		vkDeviceWaitIdle(m_LogicalDevice);
	}

	void Renderer::createVulkanInstance(glm::ivec2 size, const char* title, bool resizable, glm::ivec3 appversion) {
		if (glfwInit() == GLFW_FALSE) VUL_LOG_CORE_CRITICAL("Failed to initialize GLFW!");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		if (!resizable) glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef VUL_CONF_DEBUG
		if (!Utils::checkValidationLayersSupport()) VUL_LOG_CORE_CRITICAL("Validation layers requested, but not available!");
#endif

		uint32_t vulkanExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, nullptr);
		std::vector<VkExtensionProperties> vulkanExtensions(vulkanExtensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &vulkanExtensionCount, vulkanExtensions.data());

		for (const VkExtensionProperties& vulkanExtension : vulkanExtensions) VUL_LOG_CORE_TRACE("Available Vulkan extension: {}", vulkanExtension.extensionName);

		std::vector<const char*> glfwExtensions = Utils::getRequiredExtensions();

		for (const char* glfwExtension : glfwExtensions) VUL_LOG_CORE_TRACE("Required GLFW extension: {}", glfwExtension);

		VkApplicationInfo appInfo;
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = title;
		appInfo.pNext = nullptr;
		appInfo.applicationVersion = VK_MAKE_VERSION(appversion.x, appversion.y, appversion.z);
		appInfo.pEngineName = VUL_ENGINE_NAME;
		appInfo.engineVersion = VK_MAKE_VERSION(VUL_ENGINE_VERSION_MAJOR, VUL_ENGINE_VERSION_MINOR, VUL_ENGINE_VERSION_PATCH);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
#ifdef VUL_CONF_DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(Utils::validationLayers.size());
		createInfo.ppEnabledLayerNames = Utils::validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
#endif
		createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
		createInfo.ppEnabledExtensionNames = glfwExtensions.data();

		if (vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to initialize Vulkan!");
	}

#ifdef VUL_CONF_DEBUG
	void Renderer::setupDebugMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = Utils::debugCallback;

		if (Utils::CreateDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to set up debug messenger!");
	}
#endif

	void Renderer::pickPhysicalDevice(const Graphics::GPURequirements& gpuRequirements) {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, nullptr);
		if (deviceCount == 0) VUL_LOG_CORE_CRITICAL("Failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, devices.data());

		std::multimap<uint32_t, VkPhysicalDevice> candidates;

		for (const VkPhysicalDevice& device : devices) {
			if (Utils::isDeviceSuitable(device, m_Surface)) candidates.insert(std::make_pair(Utils::rateDeviceSuitability(device, m_Surface, gpuRequirements), device));
		}

		if (candidates.rbegin()->first > 0) m_PhysicalDevice = candidates.rbegin()->second;
		else VUL_LOG_CORE_CRITICAL("Failed to find a suitable GPU!");
	}

	void Renderer::createLogicalDevice() {
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice, m_Surface);

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
		createInfo.enabledExtensionCount = static_cast<uint32_t>(Utils::deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = Utils::deviceExtensions.data();

#ifdef VUL_CONF_DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(Utils::validationLayers.size());
		createInfo.ppEnabledLayerNames = Utils::validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
#endif

		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_LogicalDevice) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create logical device!");

		vkGetDeviceQueue(m_LogicalDevice, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicalDevice, indices.PresentFamily.value(), 0, &m_PresentQueue);
	}

	void Renderer::createSurface() {
		if (glfwCreateWindowSurface(m_VulkanInstance, m_Window->GetRaw(), nullptr, &m_Surface) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create window surface!");
	}

	void Renderer::createSwapChain(bool preferEnergyEfficiency) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice, m_Surface);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes, preferEnergyEfficiency);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities, m_Window->GetRaw());

		uint32_t imageCount = swapChainSupport.Capabilities.minImageCount + 1;

		if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount) {
			imageCount = swapChainSupport.Capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice, m_Surface);

		if (indices.GraphicsFamily == indices.PresentFamily) {
			VUL_LOG_CORE_TRACE("indices.GraphicsFamily == indices.PresentFamily, using VK_SHARING_MODE_EXCLUSIVE sharing mode!");
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		} else {
			VUL_LOG_CORE_TRACE("indices.GraphicsFamily != indices.PresentFamily, using VK_SHARING_MODE_CONCURRENT sharing mode!");
			uint32_t queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}

		createInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_LogicalDevice, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create swap chain!");

		vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageCount, nullptr);
		m_SwapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_LogicalDevice, m_SwapChain, &imageCount, m_SwapChainImages.data());

		m_SwapChainImageFormat = surfaceFormat.format;
		m_SwapChainExtent = extent;
	}

	void Renderer::createImageViews() {
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_SwapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_SwapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_LogicalDevice, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create image views!");
		}
	}

	void Renderer::createRenderPass() {
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(m_LogicalDevice, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create render pass!");
	}

	void Renderer::createGraphicsPipeline() {
		// Vertex shader
		Filesystem::File* vertexShaderFile = new Filesystem::File("Assets/Shaders/triangleShader-vert.spv");
		std::vector<char> vertShaderCode = vertexShaderFile->Read();
		delete vertexShaderFile;
		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		// Fragment shader
		Filesystem::File* fragmentShaderFile = new Filesystem::File("Assets/Shaders/triangleShader-frag.spv");
		std::vector<char> fragShaderCode = fragmentShaderFile->Read();
		delete fragmentShaderFile;
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;

		std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if (vkCreatePipelineLayout(m_LogicalDevice, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;

		if (vkCreateGraphicsPipelines(m_LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("failed to create graphics pipeline!");

		vkDestroyShaderModule(m_LogicalDevice, fragShaderModule, nullptr);
		vkDestroyShaderModule(m_LogicalDevice, vertShaderModule, nullptr);
	}

	VkShaderModule Renderer::createShaderModule(const std::vector<char>& code) {
		VkShaderModule shaderModule;

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		
		if (vkCreateShaderModule(m_LogicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create shader module!");

		return shaderModule;
	}

	void Renderer::createFramebuffers() {
		m_SwapChainFramebuffers.resize(m_SwapChainImageViews.size());

		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
			VkImageView attachments[] = {
				m_SwapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_SwapChainExtent.width;
			framebufferInfo.height = m_SwapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(m_LogicalDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create framebuffer!");
		}
	}

	void Renderer::createCommandPool() {
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice, m_Surface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

		if (vkCreateCommandPool(m_LogicalDevice, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create command pool!");
	}

	void Renderer::createCommandBuffer() {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(m_LogicalDevice, &allocInfo, &m_CommandBuffer) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to allocate command buffers!");
	}

	void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.framebuffer = m_SwapChainFramebuffers[imageIndex];

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChainExtent;

		//VkClearValue clearColorRGBA = { {{clearColor.r, clearColor.g, clearColor.b, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &m_ClearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChainExtent.width);
		viewport.height = static_cast<float>(m_SwapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to record command buffer!");
	}

	void Renderer::createSyncObjects() {
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(m_LogicalDevice, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore) != VK_SUCCESS || vkCreateSemaphore(m_LogicalDevice, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore) != VK_SUCCESS || vkCreateFence(m_LogicalDevice, &fenceInfo, nullptr, &m_InFlightFence) != VK_SUCCESS) VUL_LOG_CORE_CRITICAL("Failed to create semaphores!");
	}
}