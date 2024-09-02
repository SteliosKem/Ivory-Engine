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
IncludeDir["ImGui"] = "Ivory/vendor/imgui"
IncludeDir["glm"] = "Ivory/vendor/glm"
IncludeDir["stb"] = "Ivory/vendor/stb"
IncludeDir["entt"] = "Ivory/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Ivory/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Ivory/vendor/ImGuizmo"
IncludeDir["Alchemist"] = "Ivory/vendor/Alchemist/src"

include "Ivory/vendor/GLFW"
include "Ivory/vendor/Glad"
include "Ivory/vendor/imgui"
include "Ivory/vendor/yaml-cpp"
include "Ivory/vendor/Alchemist"

project "Ivory"
	location "Ivory"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Ivory/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/ImGui/**.ttf",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Alchemist}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"yaml-cpp",
		"Alchemist"
	}

	filter "Ivory/vendor/ImGuizmo/**.cpp"
	flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		defines {
			"IV_PLATFORM_WINDOWS",
			"IV_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "IV_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "IV_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "IV_DIST"
		runtime "Release"
		optimize "on"

project "Editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ivory/vendor/spdlog/include",
		"Ivory/src",
		"Ivory/vendor/imgui",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.entt}"
	}

	links {
		"Ivory"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"IV_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "IV_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "IV_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "IV_DIST"
		runtime "Release"
		optimize "on"