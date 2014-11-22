local META = GetMeta("Panel")

function META:Init()
  self.size = Vec2(32, 32)
  self.pos = Vec2(0, 0)
  self.bgColor = Color(100, 100, 100)
  self.color = Color(180, 180, 180)
  self.visible = true
  self.valid = true
  self.children = {}
end

function META:OnDestroy()

end

function META:Destroy()
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

function META:DrawPos()
  if self.parent then
    return self.pos + self.parent:DrawPos()
  else
    return self.pos
  end
end

function META:Delete()

end

function META:Prepare()

end

function META:Hovered()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
end
