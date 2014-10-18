project "lua"

	SetupNativeDependencyProject()
		
	local version = "5.2"
	local repo = ""
	local license = ""
	
  kind "StaticLib"
  language "C"
  files { "src/*.c" }
  includedirs { "include" }

  excludes
  {
  "src/lua.c",
  "src/luac.c"
  }

  configuration "Debug"
  	flags { "Symbols" }

  configuration "Release"
  	flags { "Optimize" }