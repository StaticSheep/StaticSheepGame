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