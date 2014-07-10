--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface       
                  Lua spaces                    
----------------------------------------------------------------------]]

local META = GetMeta("LuaSpace")

function META:CreateObject(type, name)
  CreateLuaObject(self._name, type, name)
end

function ReloadObjects()
  for space_name, space in pairs(LuaSpaces) do
    for object_id, object in pairs(space) do
      if type(object) == "table" then
        object:SetupHooks()
      end
    end
  end
end

function CreateLuaSpace(name)
  print("Created LuaSpace: "..name)
  LuaSpaces[name] = setmetatable({}, META)
  LuaSpaces[name]._name = name

  return LuaSpaces[name]
end

function DestroyLuaSpace(name)
  print("Destroyed LuaSpace: "..name)
  LuaSpaces[name] = nil
  collectgarbage()
end

function CreateLuaObject(space, type, name)
  local META = GetMeta(type)

  if (name == nil) then
    name = #LuaSpaces[space] + 1
  end

  LuaSpaces[space][name] = META.__New()

  local object = LuaSpaces[space][name]

  object:Init()

  return object
end
