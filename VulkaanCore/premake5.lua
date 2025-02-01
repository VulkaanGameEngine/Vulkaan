project "VulkaanCore"
    kind "StaticLib"

    VulkaanProject()
    
    removelinks "VulkaanCore"

    filter "configurations:not Debug-*"
        removeincludedirs { "%{wks.location}/VulkaanCore/src/debug" }
    
    filter "configurations:not Release-*"
        removeincludedirs { "%{wks.location}/VulkaanCore/src/release" }
    
    filter "configurations:not Dist-*"
        removeincludedirs { "%{wks.location}/VulkaanCore/src/dist" }
    
    filter "system:not Windows"
        removeincludedirs { "%{wks.location}/VulkaanCore/src/windows" }
    
    filter "configurations:not *-Vulkan"
        removeincludedirs { "%{wks.location}/VulkaanCore/src/vulkan" }