--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface       
                  Game spaces                    
----------------------------------------------------------------------]]

function AddGameSpace(name)
  Spaces[name] = {}
end

function RemoveGameSpace(name)
  Spaces[name] = nil
end

function RemoveGameObject(space, guid)
  Spaces[space][guid] = nil
end

function RemoveComponentFromGameObject(space, guid, cid)
  Spaces[space][guid][cid] = nil
end

-- Attaches a component to an object
-- If the object does not exist in the lua envioronment then it is created
-- An object consists of a data table of components
-- space = name of game space
-- owner = handle of the owning object
-- guid = guid of the owning object
-- cid = guid of the component
-- cname = name of the component
function AttachComponentToObject(space, owner, guid, cid, cname)

  -- Try to find the object
  local object = Spaces[space][owner]

  -- Create the object if it doesn't exist
  if not object then
    Spaces[space][guid] = {}
    object = Spaces[space][guid]
  end

  -- Create a table for the object
  object[cid] = {}

  local component = object[cid]
  setmetatable(component, _R[cname])

  component.owner = owner
  component.space = space
end