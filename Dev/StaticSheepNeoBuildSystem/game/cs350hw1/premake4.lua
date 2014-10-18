-- //--------------------------------------//
--   chart - cs350 : hw1
-- //--------------------------------------// 
project "cs350hw1"

	kind "ConsoleApp"

	SetupNativeProjects()

	pchheader "API.h"
	pchsource "platform/Main.cpp"

	files
	{
		-- local assignment files
		"*.lua"
		, "**.h"
		, "**.cpp"

		-- glsl files
		, "../../bin/shaders/*.glsl"
	}

	includedirs
	{
		-- hack needed because this script is an extra dir deeper than the invoking "src/premake4.lua"
		"../cs350hw1"

		-- collapse header dir structure
		, "platform"

		, Core.includedirs
	}

	deps
	{
		"ditfw"
		, "wxpropgrid"
		, Core.deps
	}

	defines( Core.defines );
	libdirs( Core.libdirs );

	configuration "windows"
		includedirs( Core.msvcincludedirs );
		defines( Core.msvcdefines );
		links( Core.msvclinks );

	configuration "Debug"
		defines
		{
			"DEBUG"
			, "_DEBUG"
			, "MEMORY_DEBUGGING"
			,"_HAS_ITERATOR_DEBUGGING=1"
			, "_SECURE_SCL=1"
			, "_ITERATOR_DEBUG_LEVEL=2"
		}

		flags { "Symbols" }

	configuration "Release"
		defines {"NDEBUG", "RELEASE"}
		flags {"Optimize"}
