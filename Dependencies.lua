group "Dependencies"
include "vendors/glfw"
include "vendors/glm"
include "vendors/spdlog"
group ""

IncludeDirs.Vulkan = os.getenv("VULKAN_SDK") .. "\\Include"
LibraryDirs.Vulkan = os.getenv("VULKAN_SDK") .. "\\Lib"