local META = GetMeta("Label")

function META:Init()
  self.text = ""
  self.font = "Arial"
  self.fontSize = 12
end

function META:SetSize(x, y)
  if x.x then
    self.fontSize = x.x
  else
    self.fontSize = x
  end
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

function META:Paint()
  local pos = self:DrawPos()

  draw.SimpleText(self.text, self.font, pos.x, pos.y,
   self.fontSize, self.color, 0, 0)
end

gui.Register( "Label", META, "Panel" )