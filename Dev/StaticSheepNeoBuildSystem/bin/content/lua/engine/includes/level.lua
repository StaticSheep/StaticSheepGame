--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface       
                  Lua Level Loading                    
----------------------------------------------------------------------]]

function LoadLuaLevel(path)
  filesystem.LoadSingleLuaFile(path, true)
end