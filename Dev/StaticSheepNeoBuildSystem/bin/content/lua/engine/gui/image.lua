--[[*****************************************************************
Filename: image.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("Image")

function META:Init()
  self:SetTexture("Default.png")
  self:SetColor(Color(255, 255, 255))
end

function META:SetTexture(tex)
  self.texture = tex
  self._textureID = surface.GetTextureID(self.texture)
end

function META:Paint()
  --print("Paint Image")
  local pos = self:DrawPos()



  surface.SetColor(self.color.r, self.color.g, self.color.b, self.color.a)
  surface.SetTexture(self._textureID)

  surface.DrawTexturedRectRotated(pos.x, pos.y, self.size.x, self.size.y, 0)
end

gui.Register( "Image", META, "Frame" )