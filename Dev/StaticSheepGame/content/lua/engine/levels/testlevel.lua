function foo()

  -- Clear any gamespaces
  if (gspace == nil) then
  	gspace = engine.CreateSpace("space1")
  else
  	gspace:Clear()
  end

  local obj = gspace:CreateObject("drawtest")
  local tr = obj.Transform
  local pos = Vector2(100, 0)
  tr:SetTranslation(pos)
  local size = Vector2(56, 128)
  tr:SetScale(size)

  obj = gspace:CreateObject("drawtest")
  tr = obj.Transform
  pos = Vector2(-0, 0)
  tr:SetTranslation(pos)
  size = Vector2(56, 128)
  tr:SetScale(size)

end


foo()