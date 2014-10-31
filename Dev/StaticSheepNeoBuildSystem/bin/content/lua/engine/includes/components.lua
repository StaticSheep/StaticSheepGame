
function RegisterComponent(meta)
  print("Registered LuaComponent: "..meta.__name)
  if LuaComponents[meta.__name] == nil then
    LuaComponents[meta.__name] = true
  end
end