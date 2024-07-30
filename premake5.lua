workspace "Ivory"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Ivory/vendor/GLFW/include"
IncludeDir["Glad"] = "Ivory/vendor/Glad/include"

include "Ivory/vendor/GLFW"
include "Ivory/vendor/Glad"

project "Ivory"
	location "Ivory"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Ivory/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"IV_PLATFORM_WINDOWS",
			"IV_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "IV_DEBUG"
		staticruntime "off"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "IV_RELEASE"
		staticruntime "off"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "IV_DIST"
		staticruntime "off"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ivory/vendor/spdlog/include",
		"Ivory/src"
	}

	links {
		"Ivory"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"IV_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IV_DEBUG"
		staticruntime "off"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "IV_RELEASE"
		staticruntime "off"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "IV_DIST"
		staticruntime "off"
		runtime "Release"
		optimize "On"