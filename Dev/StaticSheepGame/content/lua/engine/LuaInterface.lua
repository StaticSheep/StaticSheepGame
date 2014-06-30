
SPACES = {}

function Test()
  print("test")
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

        if(ext == "lua" and name != "LuaInterface") then
          -- Load this file into the global environment
          dofile( filePath )
        end

      elseif attr.mode == "directory" then
        -- Recursively search all subdirectories
        LoadLuaFiles( filePath .. "/" )
      end
    end
  end
end