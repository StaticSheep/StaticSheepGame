--[[*****************************************************************
Filename: luamanager.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]


function LoadLuaLevel(path)
  filesystem.LoadSingleLuaFile(path, true)
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
  if LuaSpaces[name] ~= nil then
    DestroyLuaSpace(name)
  end

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
