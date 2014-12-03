-- //--------------------------------------//
--   Static Sheep - Sheep ENgine
-- //--------------------------------------// 
project "SheepGraphics"

	kind "StaticLib"

	SetupNativeProjects()

	pchheader "precompiled.h"
	pchsource "precompiled.cpp"

	files
	{
		-- local assignment files
		"**.h",
		"**.cpp",
		"../../bin/content/shaders/*.hlsl",
		"include/SheepGraphics/api.h",

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
		"../SheepGraphics",
		"../SheepGraphics/include",
    "../SheepUtil/include",

		--"../cs350hw1"

		-- collapse header dir structure
		"platform",

    depsdir .. "/FW1FontWrapper/include",

		--Core.includedirs
	}

	deps
	{
    "SheepUtil",
		--"ditfw",
		"dxtk",
		--"wxpropgrid",
    "FW1FontWrapper",
    --"lua",
		--Core.deps
	}

	defines
  {
    --Core.defines,
    "SHEEPGRAPHICS",
    -- Definesd
  }

  postbuildcommands 
  {
  	"..\\..\\..\\tools\\AutoHeaderVS.exe $(SolutionDir)..\\..\\game\\SheepGraphics\\ $(SolutionDir)..\\..\\game\\SheepGraphics\\include\\SheepGraphics\\ GFX_API",
    "..\\..\\..\\tools\\AutoHeaderVS.exe $(SolutionDir)..\\..\\game\\SheepGraphics\\Context\\ $(SolutionDir)..\\..\\game\\SheepGraphics\\include\\SheepGraphics\\ GFX_API",
	  "..\\..\\..\\tools\\AutoHeaderVS.exe $(SolutionDir)..\\..\\game\\SheepGraphics\\Texture\\ $(SolutionDir)..\\..\\game\\SheepGraphics\\include\\SheepGraphics\\ GFX_API",
    "..\\..\\..\\tools\\AutoHeaderVS.exe $(SolutionDir)..\\..\\game\\SheepGraphics\\Camera\\ $(SolutionDir)..\\..\\game\\SheepGraphics\\include\\SheepGraphics\\ GFX_API",
    "..\\..\\..\\tools\\AutoHeaderVS.exe $(SolutionDir)..\\..\\game\\SheepGraphics\\Effects\\ $(SolutionDir)..\\..\\game\\SheepGraphics\\include\\SheepGraphics\\ GFX_API",

  }



	libdirs
  {
    Core.libdirs,
    depsdir .. "/FW1FontWrapper/x86",
    "$(DXSDK_DIR)Lib/x86",
    -- Libs
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