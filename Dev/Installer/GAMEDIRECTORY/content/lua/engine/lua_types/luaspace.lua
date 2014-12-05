--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface       
                  Lua spaces                    
----------------------------------------------------------------------]]

local META = GetMeta("LuaSpace")

function META:CreateObject(type, name)
  CreateLuaObject(self._name, type, name)
end


--  name std::string "Bob Junior"
--ach_test_type2
--  name std::string "Bob Junior2"

