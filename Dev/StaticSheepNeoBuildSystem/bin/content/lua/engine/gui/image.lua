local META = GetMeta("Image")

function META:Init()
  self:SetTexture("Default.png")
end

function META:SetTexture(tex)
  self.texture = tex
  self._textureID = surface.GetTextureID(self.texture)
end

function META:Paint()
  local pos = self:DrawPos()

  surface.DrawTexturedRectRotated(pos.x, pos.y, self.size.x, self.size.y, 0)
end

gui.Register( "Image", META, "Frame" )