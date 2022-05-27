workspace "Wallpaper"
	architecture "x86_64"
	startproject "Wallpaper"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
OPENCV_DIR = os.getenv("OPENCV_DIR")

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Glad"] = "%{wks.location}/Wallpaper/vendor/Glad/include"

group "Dependencies"
	include "Wallpaper/vendor/Glad"
group ""

project "Wallpaper"
	location "Wallpaper"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Wallpaper/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.Glad}",
		"%{OPENCV_DIR}/include/"
	}

	links 
	{
		"Glad",
		"opengl32.lib",
		"%{OPENCV_DIR}/x64/vc15/lib/*"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

	filter "configurations:Debug"
		defines
		{
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines
		{
		}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines
		{
		}
		runtime "Release"
		optimize "on"

