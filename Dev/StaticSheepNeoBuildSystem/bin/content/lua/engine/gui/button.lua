local META = GetMeta("Button")

function META:Init()
  self.OnPressed = nil

  self.text = ""
  self.font = "Arial"
  self.fontSize = 24

  self.fontColor = Color(255, 255, 255, 255)

  self.hoverColor = Color(200, 150, 150, 200)
  self.hoverBGColor = Color(150, 250, 150, 200)
  self.hoverfontColor = Color(255, 255, 255, 255)

  self.clickColor = Color(255, 255, 255)
  self.clickBGColor = Color(255, 255, 255)
  self.clickFontColor = Color(255, 255, 255)

  self.hovered = false
  self.clicked = false
  self.clickColor = 0
end

function META:SetFontColor(col)
  self.fontColor = col
end

function META:SetHoverFontColor(col)
  self.hoverfontColor = col
end

function META:SetHoverColor(col)
  self.hoverColor = col
end

function META:SetHoverBGColor(col)
  self.hoverBGColor = col
end

function META:SetClickColor(col)
  self.clickColor = col
end

function META:SetClickBGColor(col)
  self.clickBGColor = col
end

function META:SetClickFontColor(col)
  self.clickFontColor = col
end

function META:SetText(text)
  self.text = text
end

function META:GetText()
  return self.text
end

function META:SetFont(font)
  self.font = font
end

function META:GetFont()
  return self.font
end

function META:SetFontSize(fsize)
  self.fontSize = fsize
end

function META:GetFontSize()
  return self.fontSize
end

function META:SetHovered(isHovered)
  self.hovered = isHovered
end

function META:IsHovered()
  return self.hovered
end

function META:Think()
  if self.clicked then
    if self.clickedOn + 100 < CurTime() then
      self.clicked = false
    end
  end

end

function META:Press()
  if self.OnPressed then
    self:OnPressed()
  end

  self.clicked = true
  self.clickedOn = CurTime()
end

function META:SetOnPressed(func)
  self.OnPressed = func
end

function META:Paint()
  local border = 4
  local pos = self:DrawPos()

  local bgColor = self.bgColor
  local fgColor = self.color
  local fntColor = self.fontColor

  if self.hovered then
    bgColor = self.hoverBGColor
    fgColor = self.hoverColor
    fntColor = self.hoverfontColor
  end

  if self.clicked then
    bgColor = self.clickBGColor
    fgColor = self.clickColor
    fntColor = self.clickFontColor
  end

  --print(self.size.x.."x"..self.size.y)

  draw.RoundedBox(4, pos.x, pos.y,
    self.size.x, self.size.y, bgColor)

  draw.RoundedBox(4, border + pos.x, border + pos.y,
    self.size.x - border * 2, self.size.y - border * 2, fgColor)

  draw.SimpleText(self.text, self.font, pos.x + self.size.x / 2,
    pos.y + self.size.y / 2, self.fontSize, fntColor,
    TEXT_ALIGN_CENTER, TEXT_ALIGN_CENTER)
end

gui.Register( "Button", META, "Panel" )