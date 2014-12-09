--[[*****************************************************************
Filename: testlevel.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]

function foo()
  engine.Wipe()
  -- Clear any gamespaces

  local gspace = engine.CreateSpace("space1")
  local obj = gspace:CreateObject("drawtest")
  
  obj.SoundEmitter:Play("space_brawl")
  -- local tr = obj.Transform
  -- obj.Transform:SetScale(Vec2(25, 25))


end


function Draw()
  surface.SetColorEx(1,1,1,1)
  surface.DrawRect(0, 0, 12, 128)
end

hook.Add("Draw", "TestLevelDraw", Draw)


foo()