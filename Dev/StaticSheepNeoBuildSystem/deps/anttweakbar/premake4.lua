project "AntTweakBarStatic"

	SetupNativeDependencyProject()
		
	local version = ""
	local repo = ""
	local license = ""

  defines
  {
    "TW_STATIC",
    "TW_EXPORTS",
    -- Defines
  }
	
  kind "StaticLib"
  language "C++"
  files { "src/*.cpp", "src/*.c", "src/*.h" }
  includedirs { "include", "src" }

  configuration "Debug"
  	flags { "Symbols" }
    defines
    {
      "DEBUG"
      , "_DEBUG"
      , "MEMORY_DEBUGGING"
      ,"_HAS_ITERATOR_DEBUGGING=1"
      , "_SECURE_SCL=1"
      , "_ITERATOR_DEBUG_LEVEL=2"
    }

  configuration "Release"
  	flags { "Optimize" }