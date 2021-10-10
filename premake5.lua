workspace "Crosa-Conty-3D"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Crosa-Conty-3D"
    location "Crosa-Conty-3D"
    kind "SharedLib"
    language "c++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vender/spdlog/include",
        "Crosa-Conty-3D/src"
    }

    filter "system.windows"
        cppdialect "c++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CC3D_PLATFORM_WINDOWS",
            "CC3D_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buldtarget.repath}  ../bin/" .. outputdir .. "/Sandbox")
        }


    filter "configurations:Debug"
        defines "CC3D_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CC3D_RELEASE"
        symbols "On"

    filter "configurations:Dist"
        defines "CC3D_DIST"
        symbols "On"