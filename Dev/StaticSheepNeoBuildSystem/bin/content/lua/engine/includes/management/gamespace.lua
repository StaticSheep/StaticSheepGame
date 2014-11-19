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
  if GameSpaces[space][handle] == nil then return end

  local object = GameSpaces[space][handle]

  for k, comp in pairs(object) do
    if type(comp) == "table" then
      if comp.Remove then
        comp:Remove()
      end
    end
  end

  GameSpaces[space][handle] = nil
end



function PauseGameSpace(name, paused)
  if GameSpaces[name] == nil then GameSpaces[name] = {} end
  GameSpaces[name].paused = paused
end



