local META = GetMeta("Label")

function META:Init()
  self.text = ""
  self.font = "Arial"
  self.fontSize = 12

  self.outlineWidth = 0
  self.outlineColor = Color(0, 0, 0, 255)

  self.alignX = TEXT_ALIGN_LEFT
  self.alignY = TEXT_ALIGN_TOP
end

function META:SetOutlineWidth(w)
  self.outlineWidth = w
end

function META:SetOutlineColor(col)
  self.outlineColor = col
end

function META:SetFontColor(col)
  self.color = col
end

function META:SetSize(x, y)
  if x.x then
    self.fontSize = x.x
  else
    self.fontSize = x
  end
end

function META:SetXAlignment(x)
  self.alignX = x
end

function META:SetYAlignment(y)
  self.alignY = y
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

function META:Think()

end

function META:Pressed()
  if self.OnPressed then
    self:OnPressed()
  end
end

function META:SetOnPressed(func)
  self.OnPressed = func
end

function META:DrawSize()
  return surface.MeasureString(self.text, self.fontSize, self.font)
end

function META:Paint()
  local pos = self:DrawPos()

  if self.outlineWidth > 0 then
    draw.SimpleTextOutlined(self.text, self.font, pos.x, pos.y,
     self.fontSize, self.color, self.alignX, self.alignY,
     self.outlineWidth, self.outlineColor)
  else
    draw.SimpleText(self.text, self.font, pos.x, pos.y,
     self.fontSize, self.color, self.alignX, self.alignY)
  end

end

gui.Register( "Label", META, "Panel" )