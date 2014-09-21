
function foo()
  engine.Wipe()
  -- Clear any gamespaces

  local gspace = engine.CreateSpace("space1")
  local obj = gspace:CreateObject("drawtest")
  
  obj.SoundEmitter:Play("TopGun")
  -- local tr = obj.Transform
  -- obj.Transform:SetScale(Vec2(25, 25))


end

for i=1,10 do
  print("A")
end

function Draw()
  surface.SetColorEx(1,1,1,1)
  surface.DrawRect(0, 0, 128, 128)
end

hook.Add("Draw", "TestLevelDraw", Draw)


foo()