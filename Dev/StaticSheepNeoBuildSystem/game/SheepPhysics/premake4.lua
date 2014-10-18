-- //--------------------------------------//
--   Static Sheep - Sheep Physics
-- //--------------------------------------// 
project "SheepPhysics"

	kind "StaticLib"

	SetupNativeProjects()

	pchheader "precompiled.h"
	pchsource "precompiled.cpp"

	files
	{
		-- local assignment files
		"**.h",
		"**.cpp",
		"include/SheepPhysics/api.h",

		-- hw2 files (used for figuring out model scale)
		-- , "../cs350hw1/*.h"
		-- , "../cs350hw1/*.cpp"

		-- -- glsl files
		-- , "../../bin/shaders/*.glsl"
	}

	excludes {"include/**"}

	includedirs
	{
		-- hack needed because this script is an extra dir deeper than the invoking "src/premake4.lua"
		"../SheepPhysics",
		"../SheepPhysics/include",
		"../SheepUtil/include",
		--"../cs350hw1"

		-- collapse header dir structure
		"platform",

		--Core.includedirs
	}

	deps
	{
		"SheepUtil",
		--Core.deps
	}

	defines
  {
    --Core.defines,
    "SHEEPPHYSICS",
    -- Defines
  }

	libdirs
  {
    Core.libdirs,
    -- Libs
  }

  postbuildcommands 
  {
  	"..\\..\\..\\tools\\AutoHeaderVS.exe $(SolutionDir)..\\..\\game\\SheepPhysics\\ $(SolutionDir)..\\..\\game\\SheepPhysics\\include\\SheepPhysics\\ PHY_API",
	}

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