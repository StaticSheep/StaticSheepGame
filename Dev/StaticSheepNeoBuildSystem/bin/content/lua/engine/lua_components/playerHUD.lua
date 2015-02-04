--[[*****************************************************************
Filename: playerHUD.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local PlayerHUD = GetMeta("PlayerHUD")

InheritMeta(PlayerHUD, "LuaComponent")


function PlayerHUD:CleanUp()
  for k,v in pairs(self.panels) do
    v:Destroy()
  end

  self.panels = {}
  self.base = {}
end

function PlayerHUD:Register(pnl)
  table.insert(self.panels, pnl)
end

local function HealthPaint(self)
  local pos = self:DrawPos()
  local highHP = Color(0, 255, 0)
  local midHP = Color(250, 200, 0)
  local lowHP = Color(255, 0, 0)

  local logic = self.compOwner:Owner().Level1_Logic

  local HP = logic:GetPlayerHealth(self.player - 1)
  local numLives = logic:GetPlayerLives(self.player - 1)

  self.lives:SetText("x "..numLives)

  local barColor

  local size = (self:GetSize().x / 10) - (self:GetSize().x / 40)
  for i=0,9 do
    if HP > i * 10 then
      if i < 5 then
        barColor = color.Lerp(lowHP, midHP, i / 4)
      else
        barColor = color.Lerp(midHP, highHP, (i - 5) / 5)
      end
      draw.RoundedBox(2, pos.x + i * size + (self:GetSize().x / 40) * (i + 0.5), pos.y, size, 20, barColor)
    end
  end

end

local headFiles = {"ninja_head.png", "ruiser_head.png", "europe_head.png", "america_head.png"}
local headColors = {Color(0, 255, 0), Color(255, 0, 0), Color(255, 0, 255), Color(0, 120, 255)}

function PlayerHUD:Create(i)
  self.base[i] = gui.Create("Frame")
  self.base[i]:SetPriority(0)
  self:Register(self.base[i])
  self.base[i]:SetColor(Color(50, 50, 50, 0))
  self.base[i]:SetBGColor(Color(100, 100, 100, 0))
  self.base[i]:SetBorder(2)
  self.base[i]:SetSize(150, 150)
  self.base[i]:SetPos(self.panelPositions[i].x, self.panelPositions[i].y)
  self.base[i]:SetCamMode(2)

  local head = gui.Create("Image", self.base[i])
  self:Register(head)
  head:SetTexture(headFiles[i])
  head:SetSize(80, 80)
  head:SetColor(headColors[i])

  local lives = gui.Create("Label", self.base[i])
  self:Register(lives)
  lives:SetSize(ScreenScale(6))
    
  lives:SetText("x 3")
  lives:SetPos(80, 80)
  lives:SetFontColor(Color(255, 255, 255))
  lives:SetYAlignment(TEXT_ALIGN_BOTTOM)

  local healthbar = gui.Create("Panel", self.base[i])
  self:Register(healthbar)
  healthbar:SetPos(5, 80 + 5)
  healthbar:SetSize(self.base[i]:GetSize().x - 10, 30)
  healthbar.Paint = HealthPaint
  healthbar.player = i
  healthbar.compOwner = self
  healthbar.lives = lives
end

function PlayerHUD:Make()
  self.panelPositions = {Vec2(0, 10), Vec2(0, ScrH() - 120), Vec2(ScrW() - 150, 10), Vec2(ScrW() - 150, ScrH() - 120)}

  for i=1,4 do
    self:Create(i)
  end
end

function PlayerHUD:Init()
  self.panels = {}
  self.base = {}

  self:Make()

  self.super.Init(self)
end

function PlayerHUD:SetupHooks()
  hook.Add("FrameUpdate", self, self.FrameUpdate)
end

function PlayerHUD:Refresh()
  --self:CleanUp()
  --self:Make()
end

function PlayerHUD:Remove()
  self:CleanUp()
  print("REMOVE")
  hook.Remove("FrameUpdate", self)
end

function PlayerHUD:FrameUpdate(deltaTime)
  if GameSpaces[self._space]._paused or GAME_WON then
    for i=1,4 do
      if not self.base[i] then return end
      self.base[i]:SetVisible(false)
    end
  else
    for i=1,4 do
      if not self.base[i] then return end
      self.base[i]:SetVisible(true)
    end
  end
end

RegisterComponent(PlayerHUD)