--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface       
                  Game spaces                    
----------------------------------------------------------------------]]

function AddGameSpace(name)
  print("Added gamespace: "..name)
  Spaces[name] = {}
  Spaces[name].paused = false
end

function RemoveGameSpace(name)
  print("Removed game space: "..name)
  Spaces[name] = nil

  collectgarbage()
end

function RemoveGameObject(space, guid)
  print("RemovedGameObject")
  Spaces[space][guid] = nil
end

function RemoveComponentFromGameObject(space, handle, cid)
  print("Removed Component from game object")
  Spaces[space][handle][cid] = nil
end

function PauseGameSpace(name, paused)
  if Spaces[name] == nil then Spaces[name] = {} end
  Spaces[name].paused = paused
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
  local object = Spaces[space][owner]

  -- Create the object if it doesn't exist
  if not object then
    Spaces[space][owner] = {}
    object = Spaces[space][owner]
  end

  -- Create a table for the object
  object[cid] = {}

  local component = object[cid]

  setmetatable(component, GetMeta(cname))

  component._owner = owner
  component._space = space
  component._cid = cid
  component._type = cname

  component:Init()
end

function DeserializeComponent(space, owner, cid, command)
  local object = Spaces[space][owner]

  if not object then return end

  local self = object[cid]
  --print(command)
  load(command)()(self)
end

function SerializeComponent(space, owner, cid, CLComp)
  local object = Spaces[space][owner]

  if not object then return end

  local component = object[cid]

  local string = "return function(self) "

  for k,v in pairs(component) do

    if type(v) ~= "table" and type(v) ~= "userdata" then
      if string.sub(k, 0, 1) != "_" then
        if type(v) == "string" then
          string = string .. "self." .. tostring(k) .. " = '" ..tostring(v) .. "';"
        else
          string = string .. "self." .. tostring(k) .. " = " ..tostring(v) .. ";"
        end
        
      end

    end

  end

  string = string .. "end"

  --print("send back")

  CLComp:SendLoadCommand(string)

  return string
end