--[[*****************************************************************
Filename: label.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("Label")

function META:Init()
  self.text = ""
  self.font = "BN_Jinx"
  self.fontID = 0
  self.fontSize = 1

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
  self.fontSize = x
end

function META:SetXAlignment(x)
  self.alignX = x
end
META.SetXAlign = META.SetXAlignment

function META:SetYAlignment(y)
  self.alignY = y
end
META.SetYAlign = META.SetYAlignment

function META:SetText(text)
  self.text = text
end

function META:GetText()
  return self.text
end

function META:SetFont(font)
  self.font = font
  self.fontID = surface.GetFontID(font)
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
  return surface.MeasureString(self.text, self.fontSize, self.fontID)
end

function META:Paint()
  --print("Paint Label")
  local pos = self:DrawPos()

  surface.SetRotation(0)

  if self.outlineWidth > 0 then
    draw.SimpleTextOutlined(self.text, self.fontID, pos.x, pos.y,
     self.fontSize, self.color, self.alignX, self.alignY,
     self.outlineWidth, self.outlineColor)
  else
    draw.SimpleText(self.text, self.fontID, pos.x, pos.y,
     self.fontSize, self.color, self.alignX, self.alignY)
  end

end

gui.Register( "Label", META, "Panel" )