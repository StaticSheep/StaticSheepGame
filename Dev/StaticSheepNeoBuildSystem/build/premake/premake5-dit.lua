-- This is the starting point of the build scripts for the project.
-- It defines the common build settings that all the projects share
-- and calls the build scripts of all the sub-projects.

newoption
{
   trigger     = "boost-dir",
   description = "Full path to the directory for the Boost distribution."
}

dofile "helpers.lua"

solution "StaticSheep"
	
	configurations { "Debug", "Release" }
	platforms { "x32", "x64" }
	--defaultplatform "x32"
	
	flags { common_flags }
	language "C++"
	framework "4.5"
	
	location (builddir)
	objdir (builddir .. "/obj/")
	targetdir (libdir)
	libdirs { libdir }
	
	debugdir (bindir)

	-- Build configuration options

	configuration "Debug"
		defines { "__DEBUG" }
		
	configuration {}

	group "core"
		dofile( srcdir .. "/premake4.lua")

	group "projects"
		print("Searching for projects...")
		IncludePremakeProjects(path.join(projectsdir,"*"))
