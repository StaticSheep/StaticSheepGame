-- //--------------------------------------//
--   Static Sheep - Sheep ENgine
-- //--------------------------------------// 
project "SheepEngine"

	kind "WindowedApp"

	SetupNativeProjects()

	pchheader "pch/precompiled.h"
	pchsource "pch/precompiled.cpp"

	files
	{
		-- local assignment files
		"*.lua"
		, "**.h"
		, "**.cpp"

		-- hw2 files (used for figuring out model scale)
		-- , "../cs350hw1/*.h"
		-- , "../cs350hw1/*.cpp"

		-- -- glsl files
		-- , "../../bin/shaders/*.glsl"
	}

	includedirs
	{
		-- hack needed because this script is an extra dir deeper than the invoking "src/premake4.lua"
		"../SheepEngine",
    "../SheepGraphics/include",
    "../SheepPhysics/include",
    "../SheepUtil/include",
		--"../cs350hw1"

		-- collapse header dir structure
		"platform",

    depsdir .. "/fmod/include",
    depsdir .. "/lua/include",
    depsdir .. "/anttweakbar/include",

		Core.includedirs
	}

	deps
	{
		"filewatcher",
    "zlib",
    "wxWidgets",
		"wxpropgrid",
    "SheepUtil",
    "ditfw",
    "lua",
    "dxtk",
    "SheepGraphics",
    "SheepPhysics",
    "AntTweakBarStatic",
    "FW1FontWrapper",
    "fmodL_vc",
    "fmodStudioL_vc",
		--Core.deps
	}

	defines
  {
    Core.defines,
    "TW_STATIC",
    -- Defines
  }

	libdirs
  {
    depsdir .. "/fmod/lib",
    Core.libdirs,
    -- Libs
  }

  postbuildcommands 
  {
    "xcopy $(SolutionDir)..\\..\\deps\\fmod\\dll\\\*.dll $(OutDir) /f /y"
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