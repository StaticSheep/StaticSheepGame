
function foo()
  engine.Wipe()
  -- Clear any gamespaces

  local gspace = engine.CreateSpace("space1")
  local obj = gspace:CreateObject("drawtest")
  local tr = obj.Transform
  -- tr:SetTranslation(Vec2(0, 0))
  -- tr:SetScale(Vec2(5, 5))
  obj.Transform:SetScale(Vec2(25, 25))

end


foo()