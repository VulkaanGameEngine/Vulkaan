outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
LibraryDirs = {}

workspace "Vulkaan"
    startproject "VulkaanSandbox"

    for k0, v0 in pairs({ "Debug", "Release", "Dist" }) do
        for k1, v1 in pairs({ "Vulkan" }) do
            configurations (v0 .. "-" .. v1)
        end
    end

    architecture "x64"

    targetdir ("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outdir .. "/%{prj.name}")

    -- Configuration filters
    filter "configurations:Debug-*"
        symbols "on"
    
    filter "configurations:Release-*"
        optimize "on"
    
    filter "configurations:Dist-*"
        optimize "full"

    -- System filters
    filter "system:windows"
        systemversion "latest"

function VulkaanProject()
    language "C++"
    cppdialect "C++17"

    files {
        "%{prj.location}/src/all/**.h",
        "%{prj.location}/src/all/**.cpp"
    }

    includedirs {
        "%{prj.location}/src/all",
        "%{wks.location}/VulkaanCore/src/all",

        IncludeDirs.GLM,
        IncludeDirs.spdlog
    }

    defines {
        "VUL_CONF_NDEBUG",
        "VUL_CONF_NRELEASE",
        "VUL_CONF_NDIST",

        "VUL_PLATFORM_NWINDOWS",

        "VUL_API_NVULKAN"
    }

    links "VulkaanCore"

    -- Configuration filters
    filter "configurations:Debug-*"
        defines { "VUL_CONF_DEBUG" }
        undefines { "VUL_CONF_NDEBUG" }
        
        files {
            "%{prj.location}/src/debug/**.h",
            "%{prj.location}/src/debug/**.cpp"
        }
        
        includedirs {
            "%{prj.location}/src/debug",
            "%{wks.location}/VulkaanCore/src/debug"
        }

    filter "configurations:Release-*"
        defines { "VUL_CONF_RELEASE" }
        undefines { "VUL_CONF_NRELEASE" }
        
        files {
            "%{prj.location}/src/release/**.h",
            "%{prj.location}/src/release/**.cpp"
        }
        
        includedirs {
            "%{prj.location}/src/release",
            "%{wks.location}/VulkaanCore/src/release"
        }
        
        optimize "on"
    
    filter "configurations:Dist-*"
        defines { "VUL_CONF_DIST" }
        undefines { "VUL_CONF_NDIST" }
        
        files {
            "%{prj.location}/src/dist/**.h",
            "%{prj.location}/src/dist/**.cpp"
        }
        
        includedirs {
            "%{prj.location}/src/dist",
            "%{wks.location}/VulkaanCore/src/dist"
        }
        
        optimize "full"
    
    -- System filters
    filter "system:Windows"
        defines { "VUL_PLATFORM_WINDOWS" }
        undefines { "VUL_PLATFORM_NWINDOWS" }

        files {
            "%{prj.location}/src/windows/**.h",
            "%{prj.location}/src/windows/**.cpp"
        }
        
        includedirs {
            "%{prj.location}/src/windows",
            "%{wks.location}/VulkaanCore/src/windows"
        }
    
    -- API filters
    filter "configurations:*-Vulkan"
        defines { "VUL_API_VULKAN" }
        undefines { "VUL_API_NVULKAN" }

        files {
            "%{prj.location}/src/vulkan/**.h",
            "%{prj.location}/src/vulkan/**.cpp"
        }

        includedirs {
            "%{prj.location}/src/vulkan",
            "%{wks.location}/VulkaanCore/src/vulkan",
            IncludeDirs.GLFW,
            IncludeDirs.Vulkan
        }

        links {
            "GLFW",
            "vulkan-1.lib"
        }

        libdirs {
            LibraryDirs.Vulkan
        }
end

function CompileVulkanShader(src, dest)
    filter { "configurations:*-Vulkan", "system:Windows" }
        os.mkdir("Assets/Shaders")

        prebuildcommands {
            os.getenv("VULKAN_SDK") .. "\\Bin\\glslc.exe " .. src .. " -o Assets/Shaders/" .. dest
        }
end

include "Dependencies"

-- Projects
include "VulkaanCore"
include "VulkaanSandbox"