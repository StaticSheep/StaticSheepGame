Core = {}
Core.libdirs = {}
Core.deps = {}

-- global config
Core.inlcudedirs = {}
Core.defines = {}
Core.links = {}

-- msw configs
Core.msvcincludedirs = {}
Core.msvcdefines = {}
Core.msvclinks = {}

project "ditfw"
	SetupNativeDependencyProject()
	
	kind "StaticLib"

	pchheader "API.h"
	pchsource "core/Core.cpp"

	files
	{
		"*.lua"
		, "**.cpp"
		, "**.hpp"
		, "**.h"
	}

	excludes
	{
		"platform/wxw/*"

		-- exclude glfw
		-- , "gfx/glfw/*"
		-- , "input/glfw/*"
		-- , "platform/glfw/*"
	}

	Core.includedirs = 
	{
		-- collapse header dir structure
		srcdir
		, srcdir .. "/core"
		, srcdir .. "/gfx"
		, srcdir .. "/input"
		, srcdir .. "/platform"

		-- collapse wxw dirs
		, srcdir .. "/gfx/wxw"
		, srcdir .. "/input/wxw"
		, srcdir .. "/platform/wxw"

		-- deps folders
		, depsdir .. "/filewatcher/include"
		, depsdir .. "/boost_1_55"
		-- , depsdir .. "/glew/glew/include"
		-- , depsdir .. "/glsdk/glfw/include"
		-- , depsdir .. "/glsdk/glload/include"
		-- , depsdir .. "/glsdk/glmesh/include"
		-- , depsdir .. "/glsdk/glutil/include"
		-- , depsdir .. "/glsdk/glm"
		, depsdir .. "/wxwidgets/include"
		, depsdir .. "/miniformat"
		, srcdir .. "/../game/SheepGraphics/include"
		, srcdir .. "/../game/SheepUtil/include"
	}

	Core.deps = 
	{
		"filewatcher"
		-- , "glew"
		-- , "glfw"
		-- , "glload"
		-- , "glmesh"
		-- , "glutil"
		, "wxWidgets"
		, "zlib"
		, "SheepGraphics"
	}

	Core.libdirs = 
	{
		depsdir .. "/boost_1_55/lib"
	}

	Core.defines = 
	{
		-- wx defines
		"USE_OPENGL=0"
		, "wxMONOLITHIC=1"
		, "wxUSE_BASE=1"
		, "wxUSE_GUI=1"
		, "wxUSE_AUI=1"
		, "wxUSE_PROPGRID=1"
		, "wxUSE_GLCANVAS=1"

		-- glew defines
		--, "GLEW_STATIC"
		--, "GLEW_MX"

		-- boost defines
		--, "BOOST_LIB_DIAGNOSTIC"

		-- ditfw defines
		, "DIT_USE_PCH"
	}

	configuration "windows"
		Core.msvcincludedirs = 
		{
			depsdir .. "/wxwidgets/include/msvc"
		}

		Core.msvcdefines = 
		{
			"__WXMSW__"
			, "wxSETUPH_PATH=wx/msw/setup.h"
			, "_CRT_SECURE_NO_WARNINGS"
			, "_CRT_SECURE_NO_DEPRECATE"
			, "_SCL_SECURE_NO_WARNINGS"
			, "WIN32"
			, "WIN32_LEAN_AND_MEAN"
			, "_WINDOWS"
		}

		Core.msvclinks = 
		{ 
			"kernel32"
			, "user32"
			, "gdi32"
			, "comdlg32"
			, "winspool"
			, "winmm"
			, "shell32"
			, "comctl32"
			, "ole32"
			, "uuid"
			, "rpcrt4"
			, "advapi32"
			, "oleaut32"
			, "odbc32"
			, "odbccp32"
			, "opengl32"
			, "glu32"
		}

		includedirs( Core.msvcincludedirs );
		defines( Core.msvcdefines );
		links( Core.msvclinks );

	configuration {}

		includedirs( Core.includedirs );
		defines( Core.defines );
		libdirs( Core.libdirs );
		links( Core.links );
		deps( Core.deps ) ;

	configuration "Debug"
		defines
		{
			"_DEBUG"
			, "MEMORY_DEBUGGING"
			,"_HAS_ITERATOR_DEBUGGING=1"
			, "_SECURE_SCL=1"
			, "_ITERATOR_DEBUG_LEVEL=2"
		}

		flags { "Symbols" }

	configuration "Release"
		defines {"NDEBUG", "RELEASE"}
		flags { "Optimize" }

	