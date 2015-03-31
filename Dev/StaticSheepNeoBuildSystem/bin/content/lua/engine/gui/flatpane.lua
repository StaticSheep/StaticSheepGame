--[[*****************************************************************
Filename: Flatpane.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("FlatPane")

function META:Init()
end

function META:Think()

end

function META:Paint()
  -- print("Paint FlatPane")
  local pos = self:DrawPos()
  local col = self.color

  surface.SetRotation(0)
  surface.SetColor(col.r, col.g, col.b, col.a)

  if self.dynamicSize then
    surface.DrawRect(ScreenScale(pos.x), ScreenScaleY(pos.y),
     ScreenScale(self.size.x), ScreenScaleY(self.size.y))
  else
    surface.DrawRect(pos.x, pos.y, self.size.x, self.size.y)
  end
  
end

gui.Register( "FlatPane", META, "Panel" )