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

local coinFile = "coin_stack.png"

local function CoinsPaint(self)
  local pos = self:DrawPos()

  local logic = self.compOwner:Owner().Level1_Logic

  local coins = logic:GetPlayerCoins(self.player - 1)

  local image = self.coinsprite

end

local headFiles = {"p1_score.png", "p2_score.png", "p3_score.png", "p4_score.png"}
local headColors = {Color(0, 200, 0, 255), Color(200, 0, 0, 255), Color(200, 0, 200, 255), Color(0, 120, 220, 255)}

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
  head:SetSize(ScreenScale(38), ScreenScale(74))
  head:SetColor(headColors[i])

  -- local coinbar = gui.Create("Panel", self.base[i])
  -- self:Register(coinbar)
  -- coinbar:SetPos(5, 80 + 5)
  -- coinbar:SetSize(self.base[i]:GetSize().x - 10, 30)
  -- coinbar.Paint = CoinsPaint
  -- coinbar.player = i
  -- coinbar.compOwner = self
  -- coinbar.lives = lives

  local coinsprite = gui.Create("Image", self.base[i])
  self:Register(coinsprite)
  if i == 1 or i == 2 then
    coinsprite:SetPos(1, ScreenScale(74), 1)
  else
    coinsprite:SetPos(1, -ScreenScale(74), 1)
  end
  coinsprite:SetSize(ScreenScale(36), ScreenScale(74))
  coinsprite:SetTexture(coinFile)
  coinsprite.coins = 0
  coinsprite.border = head

  self.coinstack[i] = coinsprite

  --coinbar.coinsprite = coinsprite
end


function PlayerHUD:Make()
  self.panelPositions = {
  Vec2(0, ScrH() - ScreenScale(74)),
  Vec2(ScrW() - ScreenScale(38), ScrH() - ScreenScale(74)),
  Vec2(ScrW() - ScreenScale(38), 0),
  Vec2(0, 0)
  }

  for i=1,4 do
    self:Create(i)
  end
end

function PlayerHUD:Init()
  self.panels = {}
  self.base = {}
  self.coinstack = {}
  self.showDelay = {0, 0, 0, 0}

  self:Make()

  self.super.Init(self)
end

function PlayerHUD:SetupHooks()
  hook.Add("FrameUpdate", self, self.FrameUpdate)
  hook.Add("ScreenResize", self, self.Refresh)
end

function PlayerHUD:Refresh()
  self:CleanUp()
  self:Make()
end

function PlayerHUD:Remove()
  self:CleanUp()

  hook.Remove("FrameUpdate", self)
  hook.Remove("ScreenResize", self)
end

local SPAZ_COLORS = {
  Color(255, 50, 0), Color(255, 50, 255), Color(150, 255, 50), Color(150, 150, 255),
  Color(255, 255, 255), Color(0, 255, 0), Color(50, 255, 255), Color(255, 150, 20)
}

local MAX_COINS = 50000
local BOT_MIN = ScreenScale(72)
local BOT_MAX = 0
local TOP_MIN = -ScreenScale(72)
local TOP_MAX = 0

local MIN_ALPHA = 250
local ALPHA_DEC_AMOUNT = 205 / 60 / 2

local DEC_AMOUNT = MAX_COINS / 60 / 10
local INC_AMOUNT = MAX_COINS / 60 / 10

function PlayerHUD:FrameUpdate(deltaTime)

  local logic = self:Owner().Level1_Logic
  local pcoins
  local t
  local curColor

  local stack

  for i=1,4 do
    pcoins = logic:GetPlayerCoins(i - 1)
    stack = self.coinstack[i]

    if stack.coins < pcoins then
      stack.coins = math.Approach(stack.coins, pcoins, INC_AMOUNT)

      stack:SetColor(SPAZ_COLORS[math.random(#SPAZ_COLORS)])
      
      curColor = headColors[i]
      curColor.a = 255
      stack.border:SetColor(curColor)

      self.showDelay[i] = 3
    elseif stack.coins > pcoins then
      stack.coins = math.Approach(stack.coins, pcoins, DEC_AMOUNT)

      stack:SetColor(SPAZ_COLORS[math.random(#SPAZ_COLORS)])

      curColor = headColors[i]
      curColor.a = 255
      stack.border:SetColor(curColor)

      self.showDelay[i] = 3
    else
      self.showDelay[i] = self.showDelay[i] - deltaTime

      if self.showDelay[i] < 0 then
        curColor = stack:GetColor()
        curColor.a = math.Approach(curColor.a, MIN_ALPHA, ALPHA_DEC_AMOUNT)
        stack:SetColor(curColor)

        curColor = stack.border:GetColor()
        curColor.a = math.Approach(curColor.a, MIN_ALPHA, ALPHA_DEC_AMOUNT)
        stack.border:SetColor(curColor)
      else
        stack:SetColor(Color(255, 255, 255, 255))
      end
    end

    t = stack.coins / MAX_COINS
    if t > 1.0 then
      t = 1.0
    end
    if t < 0.0 then
      t = 0.0
    end

    if i == 1 or i == 2 then
      self.coinstack[i]:SetPos(1, math.lerp(BOT_MIN, BOT_MAX, t), 1)
    else
      self.coinstack[i]:SetPos(1, math.lerp(TOP_MIN, TOP_MAX, t), 1)
    end


  end


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