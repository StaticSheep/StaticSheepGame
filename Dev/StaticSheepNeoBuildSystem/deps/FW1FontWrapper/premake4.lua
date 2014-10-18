project "FW1FontWrapper"

	SetupNativeDependencyProject()
		
	local version = ""
	local repo = ""
	
  kind "StaticLib"
  
  files 
  {
  	"src/**",
  }

  includedirs { "include", "src" }
  
  defines { }

  configuration "windows"
  	files { }

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
    flags { "Optimize" }