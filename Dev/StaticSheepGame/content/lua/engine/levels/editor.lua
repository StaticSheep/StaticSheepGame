-- Static Sheep Editor --
print("\n=== Loading Editor Lua Level ===")

local gspace = engine.CreateSpace("test")
local object = gspace:CreateObject("drawtest")

-- local transform = nil
-- local pos = nil

-- local function Init()
--   local gspace = engine.CreateSpace("test")
--   local object = gspace:CreateObject("drawtest")
--   transform = object:GetComponent(0)

--   pos = transform:GetPos()
--   pos.x = 0
--   transform:SetPos(pos)

--   print(transform)
-- end

-- local right = true

-- local function Update()
--   if pos == nil or transform == nil then return end
--   if right then
--     pos.x = pos.x + 0.5
--     transform:SetPos(pos)
--     if pos.x > 300 then
--       print(transform)
--       right = false
--     end
--   else
--     pos.x = pos.x - 0.5
--     transform:SetPos(pos)
--     if pos.x < -300 then
--       right = true
--     end
--   end
-- end

-- Init()


-- local function StartReload()
--   print("Start Reload2")
--   local gspace = engine.GetSpace("test")
--   gspace:Clear()
-- end

-- local function FinishReload()
--   --engine.Quit()
--   print("Reloaded")
--   print("Lua Memory: " .. collectgarbage("count").."KB")
--   --PrintTable(LuaSpaces)
-- end

-- hook.Add("ScriptStartReload", "OnHookReloadx", StartReload)
-- hook.Add("ScriptFinishReload", "OnHookReloadx", FinishReload)
-- hook.Add("LogicUpdate", "EditorUpdate", Update)
