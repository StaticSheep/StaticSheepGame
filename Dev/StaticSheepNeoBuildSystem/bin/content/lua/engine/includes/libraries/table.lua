--[[*****************************************************************
Filename: table.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]

function table.Merge(dest, source)

  for k,v in pairs(source) do
  
    if ( type(v) == 'table' and type(dest[k]) == 'table' ) then
      -- don't overwrite one table with another;
      -- instead merge them recurisvely
      table.Merge(dest[k], v)
    else
      dest[k] = v
    end
  end
  
  return dest
end
