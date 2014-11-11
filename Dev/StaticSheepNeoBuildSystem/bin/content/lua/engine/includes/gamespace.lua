--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface       
                  Game spaces                    
----------------------------------------------------------------------]]

function ReloadComponents()
  for space_name, space in pairs(GameSpaces) do
    for object_handle, object in pairs(space) do
      if type(object) == "table" then
        for component_handle, component in pairs(object) do
          if type(component) == "table" then
            if component.SetupHooks then
              component:SetupHooks()
            elseif component.OnScriptReload then
              component:OnScriptReload()
            end
          end
        end
      end
    end
  end
end

function AddGameSpace(name)
  print("Added C++ GameSpace: "..name)
  GameSpaces[name] = {}
  GameSpaces[name]._paused = false
  GameSpaces[name]._name = name
end

function RemoveGameSpace(name)
  print("Removed C++ GameSpace: "..name)
  GameSpaces[name] = nil

  collectgarbage()
end

function RemoveGameObject(space, handle)
  print("[GameSpace: "..space.."] Removed C++ GameObject: "..handle)

  if GameSpaces[space] == nil then return end

  GameSpaces[space][handle] = nil
end

function RemoveComponentFromGameObject(space, handle, chandle)
  print("[GameSpace: "..space.."] Removed LuaComponent ("..chandle..") from C++ GameObject: "..handle)
  GameSpaces[space][handle][chandle] = nil
end

function PauseGameSpace(name, paused)
  if GameSpaces[name] == nil then GameSpaces[name] = {} end
  GameSpaces[name].paused = paused
end


-- Attaches a component to an object
-- If the object does not exist in the lua envioronment then it is created
-- An object consists of a data table of components
-- space = name of game space
-- owner = handle of the owning object
-- cid = handle of the component
-- cname = name of the component
function AttachComponentToObject(space, owner, cid, cname)

  -- Try to find the object
  local object = GameSpaces[space][owner]

  -- Create the object if it doesn't exist
  if not object then
    GameSpaces[space][owner] = {}
    object = GameSpaces[space][owner]
    object._self = owner
  end

  -- Create a table for the object
  object[cid] = {}

  local component = object[cid]
  local meta = GetMeta(cname)

  PrintTable(meta)

  setmetatable(component, meta)

  component._owner = owner
  component._space = space
  component._cid = cid
  component._type = cname

  print("[GameSpace: "..space.."] Attached LuaComponent: "..cname.." ["..cid.."] to C++ GameObject: "..owner)

  component:Init()
end

function DeserializeComponent(space, owner, cid, command)
  local object = GameSpaces[space][owner]

  if not object then return end

  local self = object[cid]
  --print(command)
  load(command)()(self)
end

function SerializeComponent(space, owner, cid, CLComp)
  local object = GameSpaces[space][owner]

  if not object then return end

  local component = object[cid]

  local string = "return function(self) "

  for k,v in pairs(component) do

    if type(v) ~= "table" and type(v) ~= "userdata" then
      if string.sub(k, 0, 1) != "_" then
        if type(v) == "string" then
          string = string .. "self." .. tostring(k) .. " = '" ..tostring(v) .. "' "
        else
          string = string .. "self." .. tostring(k) .. " = " ..tostring(v) .. " "
        end
        
      end

    end

  end

  string = string .. "end"

  --string = "asdfasdfasdfffffffaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafffffaaaaafffffaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafffffaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafffffaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaafffffaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

  --print("send back")
  --PrintTable(getmetatable(CLComp))

  CLComp:SendLoadCommand(string)

  --return string
end