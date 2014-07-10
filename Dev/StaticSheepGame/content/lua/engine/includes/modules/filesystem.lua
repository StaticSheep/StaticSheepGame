--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface                           
                  File system
----------------------------------------------------------------------]]
local pairs = pairs
local print = print
local string = string
local lfs = lfs
local dofile = dofile
local ReloadComponents = ReloadComponents
local ReloadObjects = ReloadObjects

local PrintTable = PrintTable

module( "filesystem" )

function Init()
  Files = {}
  FileBlacklist = {}
  FolderBlacklist = {}
end

function BlacklistFolder(name)
  FolderBlacklist[name] = true
end

function BlacklistFile(name)
  FileBlacklist[name] = true
end

function LoadLuaFiles( path )
  for file in lfs.dir( path ) do
    if file ~= "." and file ~= ".." then
      -- Grab the file attributes
      local filePath = path .. file
      local attr = lfs.attributes( filePath )

      -- Only process files (not directories)
      if attr.mode == "file" then
        -- Retrieve the file extension
        local ext = string.match( file, "([^.]+)$" )
        -- Strip off the file extension
        local name = string.match( file, "[^.]+" )

        if(ext == "lua" and FileBlacklist[name] == nil) then
          print("Loading File: "..filePath)
          Files[#Files + 1] = {filePath, attr.modification}
          -- Load this file into the global environment
          dofile( filePath )
        end

      elseif attr.mode == "directory" then
        local name = string.match( file, "[^.]+" )

        if (FolderBlacklist[name] == nil) then
          -- Recursively search all subdirectories
          LoadLuaFiles( filePath .. "/" )
        end
      end
    end
  end
end

function LoadLuaFile(path, needle)
  for file in lfs.dir( path) do
    if file ~= "." and file ~= ".." then
      -- Grab the file attributes
      local filePath = path .. file
      local attr = lfs.attributes( filePath )

      -- Only process files (not directories)
      if attr.mode == "file" then
        -- Retrieve the file extension
        local ext = string.match( file, "([^.]+)$" )

        if(ext == "lua" and file == needle) then
          -- Load this file into the global environment
          dofile( filePath )
          return true
        end

      elseif attr.mode == "directory" then
        local name = string.match( file, "[^.]+" )

          -- Recursively search all subdirectories
          LoadLuaFile( filePath .. "/", needle )
      end
    end
  end
end

function UpdateOldFiles()
  for _, file in pairs(Files) do
    local attr = lfs.attributes(file[1])

    --PrintTable(file)
    --PrintTable(attr)

    if attr ~= nil and attr.modification > file[2] then
      print("Reloaded "..file[1])

      file[2] = attr.modification
      dofile(file[1])

      ReloadObjects()
      ReloadComponents()
    end

  end
end
