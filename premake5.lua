workspace "Calmar"
	startproject "Editor"
	architecture "x64"

	configurations 
	{
		"Debug",
		"Release",
		"Distribution"
	}

project_output = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

Dependencies = {}
Dependencies["GLFW"] = "engine/vendor/glfw/include"
Dependencies["Glad"] = "engine/vendor/glad/include"
Dependencies["ImGui"] = "engine/vendor/imgui"
Dependencies["glm"] = "engine/vendor/glm"
Dependencies["spdlog"] = "engine/vendor/spdlog/include"
Dependencies["stb_image"] = "engine/vendor/stb_image"
Dependencies["entt"] = "engine/vendor/entt/include"
Dependencies["yaml_cpp"] = "engine/vendor/yaml-cpp/include"
Dependencies["ImGuizmo"] = "engine/vendor/imguizmo"
Dependencies["Box2D"] = "engine/vendor/box2d/include"						
Dependencies["mono"] = "engine/vendor/mono/include"

group "vendor"
	include "engine/vendor/glad"
	include "engine/vendor/imgui"
	include "engine/vendor/yaml-cpp"
	include "engine/vendor/box2d"
group ""

group "calmar"

project "engine"
	location "engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. project_output .. "/%{prj.name}")
	objdir ("obj/" .. project_output .. "/%{prj.name}")

	defines
	{
		"NOMINMAX",
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE"
	}
	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glad/src/**.cpp",
		"%{prj.name}/vendor/imguizmo/ImGuizmo.h",
		"%{prj.name}/vendor/imguizmo/ImGuizmo.cpp"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{Dependencies.spdlog}",
		"%{Dependencies.GLFW}",
		"%{Dependencies.Glad}",
		"%{Dependencies.ImGui}",
		"%{Dependencies.glm}",
		"%{Dependencies.stb_image}",
		"%{Dependencies.entt}",
		"%{Dependencies.yaml_cpp}",
		"%{Dependencies.ImGuizmo}",
		"%{Dependencies.Box2D}",
		"%{Dependencies.mono}"
	}

	libdirs
	{
		"engine/vendor/glfw/lib",
		"engine/vendor/mono/lib/%{cfg.buildcfg}"
	}

	links
	{
		"Box2D",
		"GLFW.lib",
		"ImGui",
		"Glad",
		"yaml-cpp",
		"libmono-static-sgen.lib",
		"opengl32.lib",
		"Winmm.lib",
		"Version.lib",
		"Bcrypt.lib",
		"Ws2_32.lib"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CALMAR_PLATFORM_WINDOWS",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM",
			"IMGUI_API=__declspec(dllexport)"
		}

	filter "configurations:Debug"
		defines "CALMAR_DEBUG_BUILD"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "CALMAR_RELEASE_BUILD"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CALMAR_DIST_BUILD"
		runtime "Release"
		optimize "on"

project "editor"
	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. project_output .. "/%{prj.name}")
	objdir ("obj/" .. project_output .. "/%{prj.name}")
	defines
	{
		"NOMINMAX",
		"_CRT_SECURE_NO_WARNINGS"
	}
	files
	{
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{Dependencies.spdlog}",
		"engine/src",
		"engine/vendor",
		"%{Dependencies.spdlog}",
		"%{Dependencies.GLFW}",
		"%{Dependencies.Glad}",
		"%{Dependencies.ImGui}",
		"%{Dependencies.glm}",
		"%{Dependencies.stb_image}",
		"%{Dependencies.entt}",
		"%{Dependencies.yaml_cpp}",
		"%{Dependencies.ImGuizmo}",
		"%{Dependencies.Box2D}"
	}
	links
	{
		"engine"
	}
	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"BLIND_PLATFORM_WINDOWS",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}
	
	filter "configurations:Debug"
		defines "CALMAR_DEBUG_BUILD"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CALMAR_RELEASE_BUILD"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "CALMAR_DIST_BUILD"
		runtime "Release"
		optimize "on"

project "calmar_scripting"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	location "calmar_scripting"

	targetdir ("%{wks.location}/editor/res/scripting/")
	objdir ("%{wks.location}/editor/res/scripting/obj")

	files
	{
		"{prj.name}/src/**.cs",
		"{prj.name}/properties/**.cs",
	}

	filter "configurations:Debug"
		optimize "off"
		symbols "Default"

	filter "configurations:Release"
		optimize "on"
		symbols "Default"

	filter "configurations:Distribution"
		optimize "full"
		symbols "off"

group ""