-- Static Sheep Editor --
print("\n=== Loading Editor Lua Level ===")

local gspace = engine.CreateSpace("test")
local object = gspace:CreateObject("test_type2")
local transform = object:GetComponent(0)

local function Update()
  --print("Stop!")
  -- for i=0,1000 do
  --   object = gspace:CreateObject("test_type2")
  --   object:Destroy()
  -- end
end


local function Reload()
  --engine.Quit()
  print("Reloaded")
  print("Lua Memory: " .. collectgarbage("count").."KB")
  --PrintTable(LuaSpaces)
end

hook.Add("ScriptReload", "OnHookReloadx", Reload)
hook.Add("LogicUpdate", "EditorUpdate", Update)
