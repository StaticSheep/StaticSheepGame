-- Static Sheep Editor --
print("\n=== Loading Editor Lua Level ===")

local transform = nil
local pos = nil

local function Init()
  local gspace = engine.CreateSpace("test")
  -- local object = gspace:CreateObject("drawtest")
  -- object = gspace:CreateObject("drawtest")
  -- transform = object:GetComponent(0)

  -- pos = transform:GetPos()
  -- pos.x = 0
  -- transform:SetPos(pos)

  -- print(transform)
end

local right = true

local function Update()
  if pos == nil or transform == nil then return end
  if right then
    pos.x = pos.x + 0.5
    transform:SetPos(pos)
    if pos.x > 300 then
      right = false
    end
  else
    pos.x = pos.x - 0.5
    transform:SetPos(pos)
    if pos.x < -300 then
      right = true
    end
  end
end

Init()


local function StartReload()
  print("Start Reload2")
  --local gspace = engine.GetSpace("test")
  --gspace:Clear()
end

local function FinishReload()
  --engine.Quit()
  print("Reloaded")
  print("Lua Memory: " .. collectgarbage("count").."KB")
  --PrintTable(LuaSpaces)
end

hook.Add("ScriptStartReload", "OnHookReloadx", StartReload)
hook.Add("ScriptFinishReload", "OnHookReloadx", FinishReload)
--hook.Add("LogicUpdate", "EditorUpdate", Update)

local i = 0
local up = true
local dt = 0
  
local function Draw()
  -- surface.SetUseCamera(0)
  -- surface.DrawRect(0, 0, 256, 256)
  -- surface.DrawRect(0, 0, 512, 128)
  -- surface.DrawLine(50, 500 + 5 * i, 700, 200 + 5 * i, 10)


  if up then
     i = i + 1
     if i > 500 then up = false end
   else
     i = i - 1
     if i < -500 then up = true end
   end
  dt = dt + .001

texID = surface.GetTextureID("content/maverick.jpg")
  surface.SetRotation(dt)
  surface.SetTexture(texID)
  surface.SetColorEx(1,1,1,1)
  surface.DrawTexturedRect(i, 0, 200, 200)
  
  surface.SetRotation(-dt * 5)
  surface.SetColorEx(1,1,1,1)
  surface.SetTexture(texID)
  --surface.DrawRect(0, 0, 200, 200)
  surface.SetRotation(-dt)
  surface.SetColorEx(1,1,1,1)
  texID = surface.GetTextureID("content/goose.jpg")
  surface.SetTexture(texID)
  surface.DrawTexturedRect(-i, 0, 200, 200)
  surface.SetPos(0, 0)
  surface.SetColorEx(1,0,0,1)
  surface.DrawString("Top Gun!", i / 4, "")
  
  

  -- local texID2 = surface.GetTextureID("content/bricks.png")
  -- surface.SetTexture(texID2)
  -- surface.DrawTexturedRect(256, i * 50, 256, 256)



  -- surface.DrawRect(0, 0, 400, 300)

end

hook.Add("Draw", "Editor::Draw", Draw)
