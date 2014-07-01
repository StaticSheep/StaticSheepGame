--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface                           
                  File system
----------------------------------------------------------------------]]
local pairs = pairs
local print = print
local string = string
local lfs = lfs
local dofile = dofile

module( "filesystem" )

function Init()
  Files = {}
end

function LoadLuaFiles( path )
  for file in lfs.dir( path ) do
    if file != "." and file != ".." then
      -- Grab the file attributes
      local filePath = path .. file
      local attr = lfs.attributes( filePath )

      -- Only process files (not directories)
      if attr.mode == "file" then
        -- Retrieve the file extension
        local ext = string.match( file, "([^.]+)$" )
        -- Strip off the file extension
        local name = string.match( file, "[^.]+" )

        if(ext == "lua" and name != "interface") then
          print("Loading File: "..filePath)
          Files[#Files + 1] = {filePath, attr.modification}
          -- Load this file into the global environment
          dofile( filePath )
        end

      elseif attr.mode == "directory" then
        local name = string.match( file, "[^.]+" )

        if (name != "modules") then
          -- Recursively search all subdirectories
          LoadLuaFiles( filePath .. "/" )
        end
      end
    end
  end
end

function UpdateOldFiles()
  for _, file in pairs(Files) do
    local attr = lfs.attributes(file[1])
    if attr.modification > file[2] then
      file[2] = attr.modification
      dofile(file[1])
    end
  end
end
