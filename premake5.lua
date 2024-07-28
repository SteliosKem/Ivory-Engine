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

include "Ivory/vendor/GLFW"

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
		"%{IncludeDir.GLFW}"
	}

	links {
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"IV_PLATFORM_WINDOWS",
			"IV_BUILD_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "IV_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "IV_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "IV_DIST"
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
		symbols "On"

	filter "configurations:Release"
		defines "IV_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "IV_DIST"
		optimize "On"