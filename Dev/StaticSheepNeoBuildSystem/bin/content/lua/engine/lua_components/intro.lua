--[[*****************************************************************
Filename: intro.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("IntroScreen")

InheritMeta(META, "LuaComponent")


function FadeInUpdate(self, dt, data)
  self.tl = self.tl - dt
end

local function lerp(start, finish, t)
  return start + t*(finish - start)
end

local function Timed(time)
  return function(act)
  act.ttl = time
  act.tl = act.ttl
  end
end

local function ChangeTexture(self, id)
  return function(act)
    self._texID = self.tex[id]
  end
end

local function ChangeDimensions(self, size)
  return function(act)
    self._texSize = size
  end
end

local function Hold()
  return function(act, dt)
  act.tl = act.tl - dt
  if act.tl < 0 then
    act:Done()
  end
  end
end

local function FadeIn(self)
  return function(act, dt)
    act.tl = act.tl -dt
    self._alpha = lerp(0, 255, (1-(act.tl/act.ttl)))
    Hold()(act, dt)
  end
end

local function FadeOut(self)
  return function(act, dt)
    act.tl = act.tl -dt
    self._alpha = lerp(0, 255, (act.tl/act.ttl))
    Hold()(act, dt)
  end
end

local function MoveObject(self, id, start, finish)
  return function(act, dt)
    local cur = Vec3(0, 0, 0)
    cur.x = lerp(start.x, finish.x, 1 - (act.tl / act.ttl))
    cur.y = lerp(start.y, finish.y, 1 - (act.tl / act.ttl))
    cur.z = lerp(start.z, finish.z, 1 - (act.tl / act.ttl))
    self._slotMachine[id].Transform:SetTranslation(cur)
  end
end

function META:Run()
  self.tex = {}
  self.tex[1] = surface.GetTextureID("InTheFuture.png")
  self.tex[2] = surface.GetTextureID("ThereIsOnly.png")
  self.tex[3] = surface.GetTextureID("Entertainment.png")
  self.tex[4] = surface.GetTextureID("GigaGravityTitle.png")

  self._alpha = 255
  self._texID = self.tex[1]
  self._texSize = Vec2(2, 2)
  self._slotMachine = {}

  self.List = actionlist.Create()
  self.List:PushBack(Action(
    Timed(1.5),
    FadeIn(self),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(2),
    Hold(),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(1),
    FadeOut(self),
    ChangeTexture(self, 2),
    true))

  self.List:PushBack(Action(
    Timed(1.25),
    FadeIn(self),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(2.5),
    Hold(),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(1),
    FadeOut(self),
    function(act)
      ChangeTexture(self, 3)(act)
      ChangeDimensions(self, Vec2(3, 3))(act)
    end,
    true))

  self.List:PushBack(Action(
    nil,
    function(act)
      local obj = self._spacePtr:CreateObject("IntroMachine2")
      obj.Transform:SetTranslation(Vec3(0, 830, 0))
      self._slotMachine[1] = obj
      -- obj = self._spacePtr:CreateObject("IntroMachine2")
      -- obj.Transform:SetTranslation(Vec3(0, -830, 0))
      -- self._slotMachine[2] = obj
      act:Done()
    end,
    nil,
    false))

  self.List:PushBack(Action(
    Timed(1),
    FadeIn(self),
    nil,
    false))

  self.List:PushBack(Action(
    Timed(0.8),
    function(act, dt)
      MoveObject(self, 1, Vec3(0, 830, 0), Vec3(0, 270, 0))(act, dt)
      --MoveObject(self, 2, Vec3(0, -830, 0), Vec3(0, -330, 0))(act, dt)
      Hold()(act, dt)
    end,
    nil,
    true))

  self.List:PushBack(Action(
    Timed(1.1),
    FadeOut(self),
    function(act)
      ChangeTexture(self, 4)(act)
      ChangeDimensions(self, Vec2(0.95, 0.95))(act)
    end,
    true))

  self.List:PushBack(Action(
    Timed(1.25),
    FadeIn(self),
    nil,
    true))


end

function META:Init()
  self.super.Init(self)

  self:Run()
end

function META:Refresh()
  self:Run()
end

function META:SetupHooks()
  hook.Add("LogicUpdate", self, self.Update, self.CanUpdate)
  hook.Add("Draw", self, self.Draw)
end

function META:Remove()
  hook.Remove("LogicUpdate", self)
  hook.Remove("Draw", self)
end

function META:Update(dt)
  self.List:Update(dt)
end

function META:Draw()
  surface.SetTexture(self._texID)
  surface.SetColor(255, 255, 255, self._alpha)
  surface.ForceZ(true, -1.0)
  surface.DrawTexturedRect(0, 0, self._texSize.x, self._texSize.y)
  surface.ForceZ(false, 0)
end

RegisterComponent(META)