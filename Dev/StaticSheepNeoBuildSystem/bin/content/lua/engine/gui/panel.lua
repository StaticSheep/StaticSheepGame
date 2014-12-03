local META = GetMeta("Panel")

function META:Init()
  self.size = Vec2(32, 32)
  self.pos = Vec2(0, 0)
  self.bgColor = Color(100, 100, 100)
  self.color = Color(180, 180, 180)
  self.visible = true
  self.valid = true
  self.camMode = 2
  self.priority = 0
  self.children = {}
end

function META:SetPriority(p)
  self.priority = p
end

function META:OnDestroy()

end

function META:Destroy()
  --print("Destroying "..self.className)
  self:OnDestroy()

  self.valid = false

  for k,v in pairs(self.children) do
    v:Destroy()
  end
end

function META:SetColor(color)
  self.color = color
end

function META:SetBGColor(color)
  self.bgColor = color
end

function META:SetSize(w, h)
  if h == nil then
    self.size = w
  else
    self.size = Vec2(w, h)
  end
end

function META:GetSize()
  return self.size
end

function META:SetPos(x, y)
  if y == nil then
    self.pos = x
  else
    self.pos = Vec2(x, y)
  end
end

function META:GetPos()
  return self:DrawPos()
end

function META:SetCamMode(mode)
  self.camMode = mode
end

function META:GetCamMode()
  return self.camMode
end

function META:DrawPos()
  if self.parent then
    return self.pos + self.parent:DrawPos()
  else
    return self.pos
  end
end

-- Do not override this function
function META:_BaseDelete()
  --print("Deleting "..self.className)
  self:Delete()

  for k,v in pairs(self.children) do
    v:_BaseDelete()
  end
end

function META:Delete()

end

function META:Prepare()

end

function META:Hovered()

end

function META:_BaseThink()
  self:Think()

  for k,v in pairs(self.children) do
    v:_BaseThink()
  end
end

function META:Think()

end

function META:_PreDraw()
  if not self.parent then
    --surface.SetCamState(self.camMode)
  end
end

function META:_PostDraw()
  if not self.parent then
    --surface.SetCamState(0)
  end
end

function META:_BasePaint()
  self:_PreDraw()

  self:Paint()

  for k,v in pairs(self.children) do
    if v.visible then
      v:_BasePaint()
    end
  end

  self:_PostDraw()
end

function META:Paint()
  -- Do the painting yeah
end
