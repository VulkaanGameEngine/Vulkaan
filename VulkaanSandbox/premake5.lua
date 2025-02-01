project "VulkaanSandbox"
    kind "ConsoleApp"
    
    VulkaanProject()

    CompileVulkanShader("%{prj.location}/res/Vulkan/shaders/triangleShader.frag", "triangleShader-frag.spv")
    CompileVulkanShader("%{prj.location}/res/Vulkan/shaders/triangleShader.vert", "triangleShader-vert.spv")