--[[*****************************************************************
Filename: Transform.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("Transform")


function META:SetPos(pos)
  self:Settranslation(pos)
end

function META:GetPos()
  return self:Gettranslation()
end