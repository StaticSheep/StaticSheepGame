--[[*****************************************************************
Filename: basic.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("BasicObject")

function META:Init()
  self:SetupHooks()
end

function META:SetupHooks()
  hook.Add("LogicUpdate", self, self.Update)
end

function META:Update(deltaTime)
end