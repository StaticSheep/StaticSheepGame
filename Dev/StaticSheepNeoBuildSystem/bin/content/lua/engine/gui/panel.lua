local META = GetMeta("Panel")

function META:Init()
  self.size = Vec2(32, 32)
  self.pos = Vec2(0, 0)
  self.bgColor = Color(100, 100, 100)
  self.color = Color(180, 180, 180)
  self.visible = true
  self.valid = true
end

function META:Destroy()
  self.valid = false
end

function META:SetColor(color)
  self.color = color
end

function META:SetBGColor(color)
  self.bgColor = color
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
