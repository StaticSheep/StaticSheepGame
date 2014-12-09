--[[*****************************************************************
Filename: uitest.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("UITest")

InheritMeta(META, "LuaComponent")

function META:Init()
  self.MyPanel = gui.Create("Frame")

  self.MyPanel.pos = Vec2(100, 100)
  self.MyPanel.size = Vec2(512, 256)

  self.MyPanel:SetColor(Color(180, 180, 180, 255))
  self.MyPanel:SetBGColor(Color(100, 100, 100, 255))

  self.MyButton = gui.Create("Button", self.MyPanel)

  self.MyButton.pos = Vec2(10, 10)
  self.MyButton.size = Vec2(100, 20)

  self.MyButton:SetColor(Color(250, 180, 180, 255))
  self.MyButton:SetBGColor(Color(100, 100, 100, 255))

  self.super.Init(self)
end

function META:SetupHooks()
  hook.Add("LogicUpdate", self, self.Update, self.CanUpdate)
end

function META:Update(deltaTime)
end

function META:Remove()
  self.MyPanel:Destroy()
end

RegisterComponent(META)