local META = GetMeta("Button")

function META:Init()
  self.hoverColor = Color(200, 150, 150, 200)
  self.bgHoverColor = Color(150, 250, 150, 200)
  self.OnPressed = nil
  self.alignX = TEXT_ALIGN_LEFT
  self.alignY = TEXT_ALIGN_TOP

  self.hovered = false
end

function META:SetXAlignment(x)
  self.alignX = x
end

function META:SetYAlignment(y)
  self.alignY = y
end

function SetHovered(isHovered)
  self.hovered = isHovered
end

function IsHovered()
  return self.hovered
end

function META:Think()

end

function META:Press()
  if self.OnPressed then
    self:OnPressed()
  end
end

function META:SetOnPressed(func)
  self.OnPressed = func
end

function META:Paint()
  local border = 4
  local pos = self:DrawPos()


  self:SetBGColor(Color(100, 100, 100, 200))
  self:SetColor(Color(180, 180, 180, 200))
  self:SetSize(Vec2(256, 64))

  draw.RoundedBox(4, pos.x, pos.y,
    self.size.x, self.size.y, self.bgColor)

  draw.RoundedBox(4, border + pos.x, border + pos.y,
    self.size.x - border * 2, self.size.y - border * 2, self.color)
end

gui.Register( "Button", META, "Panel" )