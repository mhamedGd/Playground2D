workspace "Playground2D"
	platforms "Win32"
	architecture "x86"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "Playground2D"
	location "Playground2D"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Dependencies/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WIN32",
			"_LIB"
		}

	filter "configurations:Debug"
		defines "_DEBUG;"
		symbols "On"
		

	filter "configurations:Release"
		defines	"NDEBUG"
		optimize "On"
		


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"Dependencies/include",
		"Playground2D/src"
	}

	libdirs
	{
		"Dependencies/lib/glew",
		"Dependencies/lib/glfw",
		"Dependencies/lib/freetype"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WIN32",
			"_CONSOLE"
		}

	filter "configurations:Debug"
		defines {
			"_DEBUG",
			"PLAYGROUND_2D_DEBUG"
		}
		symbols "On"		

	filter "configurations:Release"
		defines "NDEBUG"
		optimize "On"
		
	
	filter "system:windows"
		links
		{
			"Playground2D",
			"freetype",
			"glew32s",
			"glfw3",
			"opengl32",
			"kernel32",
			"user32",
			"gdi32",
			"winspool",
			"comdlg32",
			"advapi32",
			"shell32",
			"ole32",
			"oleaut32",
			"uuid",
			"odbc32",
			"odbccp32"
		}		
		prebuildcommands {
			"{COPYDIR} $(SolutionDir)%{prj.name}/Assets $(SolutionDir)bin/" .. outputdir .. "/%{prj.name}/Assets"
		}
