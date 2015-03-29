--[[*****************************************************************
Filename: intro.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("SplashLogo")

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
    self._alpha = lerp(0, 255, (1-(act.tl/act.ttl)))
    Hold()(act, dt)
  end
end

local function FadeOut(self)
  return function(act, dt)
    self._alpha = lerp(0, 255, (act.tl/act.ttl))
    Hold()(act, dt)
  end
end

local function Translate(self, obj, start, finish)
  return function(act, dt)
    local cur = Vec3(0, 0, 0)
    local t = (act.tl / act.ttl)
    if t > 1 then t = 1 end
    if t < 0 then t = 0 end
    cur.x = lerp(start.x, finish.x, 1 - t)
    cur.y = lerp(start.y, finish.y, 1 - t)
    cur.z = lerp(start.z, finish.z, 1 - t)
    obj.Transform:SetTranslation(cur)
  end
end

local function MoveText(self, start, finish)
  return function(act, dt)
  local cur = Vec3(0, 0, 0)
  local t = (act.tl / act.ttl)
  if t > 1 then t = 1 end
  if t < 0 then t = 0 end
  cur.x = lerp(start.x, finish.x, 1 - t)
  cur.y = lerp(start.y, finish.y, 1 - t)
  cur.z = lerp(start.z, finish.z, 1 - t)
  self._textPos = cur
  end
end


function META:Run()
  self.tex = {}
  self.tex[1] = surface.GetTextureID("dp_whitelogo.png")
  self.tex[2] = surface.GetTextureID("logohead.png")

  self._alpha = 0
  self._texID = self.tex[1]
  self._texSize = Vec2(1, 1)
  self._textPos = Vec3(0, 0, 0)
  self._slotMachine = {}
  self._logo = nil
  self._startBtn = nil

  self.List = actionlist.Create()

  self.List:PushBack(Action(
    Timed(2.5),
    FadeIn(self),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(3),
    Hold(),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(1.5),
    FadeOut(self),
    function(act)
      ChangeTexture(self, 2)(act)
      ChangeDimensions(self, Vec2(1, 1))(act)
    end,
    true))

  -- self.List:PushBack(Action(
  --   Timed(1.5),
  --   FadeIn(self),
  --   nil,
  --   true))

  -- self.List:PushBack(Action(
  --   Timed(2),
  --   Hold(),
  --   nil,
  --   true))

  -- self.List:PushBack(Action(
  --   Timed(1.5),
  --   FadeOut(self),
  --   nil,
  --   true))

  self.List:PushBack(Action(
    function(act)
      engine.ChangeLevel("Intro")
    end,
    function(act, dt)

    end,
    nil,
    false))


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

  if gamepad.ButtonPressed(nil, GAMEPAD_START) then
    engine.ChangeLevel("Intro")
  end
end

function META:Draw()
  surface.SetTexture(self._texID)
  surface.SetColor(255, 255, 255, self._alpha)
  surface.ForceZ(true, self._textPos.z - 1.0)
  surface.DrawTexturedRect(self._textPos.x, self._textPos.z, self._texSize.x, self._texSize.y)
  surface.ForceZ(false, 0)
end

RegisterComponent(META)