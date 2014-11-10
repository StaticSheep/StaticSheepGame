
function RegisterComponent(meta)
  print("Registered LuaComponent: "..meta.__name)
  if LuaComponents[meta.__name] == nil then
    LuaComponents[meta.__name] = meta.__name
  end
end

function QueryComponents()
  print("Querying Lua Components")

  local tbl = {}

  for k,v in pairs(LuaComponents) do
    tbl[#tbl + 1] = v
  end

  PrintTable(LuaComponents)
  engine.SendComponentList(#tbl, tbl)
end