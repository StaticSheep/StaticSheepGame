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
    self._alpha = lerp(0, 255, (1-(act.tl/act.ttl)))
    Hold()(act, dt)
  end
end

local function FadeIn2(self)
  return function(act, dt)
    self._alpha2 = lerp(0, 255, (1-(act.tl/act.ttl)))
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
  self.tex[1] = surface.GetTextureID("InTheFuture.png")
  self.tex[2] = surface.GetTextureID("ThereIsOnly.png")
  self.tex[3] = surface.GetTextureID("Entertainment.png")
  self.tex[4] = surface.GetTextureID("GigaGravityTitle.png")
  self.tex[5] = surface.GetTextureID("dp_whitelogo.png")
  self.tex[6] = surface.GetTextureID("controllerscreen.png")

  self._alpha = 0
  self._alpha2 = 0
  self._texID = self.tex[5]
  self._texSize = Vec2(1, 1)
  self._textPos = Vec3(0, 0, 0)
  self._slotMachine = {}
  self._logo = nil
  self._startBtn = nil

  self.List = actionlist.Create()
  self.List:PushBack(Action(
    nil,
    function(act, dt)
      self:Owner().SoundPlayer:PlayLoop("factory_roar")
      act:Done()
    end,
    nil,
    false))

  self.List:PushBack(Action(
    Timed(0.5),
    FadeIn(self),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(3),
    Hold(),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(0.5),
    FadeOut(self),
    function(act)
      ChangeTexture(self, 6)(act)
      ChangeDimensions(self, Vec2(0.9, 0.9))(act)
    end,
    true))

  self.List:PushBack(Action(
    Timed(0.5),
    FadeIn(self),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(2),
    Hold(),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(1.5),
    FadeOut(self),
    function(act)
      ChangeTexture(self, 1)(act)
      ChangeDimensions(self, Vec2(2, 2))(act)
    end,
    true))

  self.List:PushBack(Action(
    Timed(0.25),
    Hold(),
    nil,
    true))

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
      ChangeDimensions(self, Vec2(4, 4))(act)
    end,
    true))

  self.List:PushBack(Action(
    Timed(0.1),
    Hold(),
    function(act, dt)
      self:Owner().SoundPlayer:StopLoop("factory_roar")
    end,
    true))

  self.List:PushBack(Action(
    function(act)
      self:Owner().SoundPlayer:PlayLoop("tripg")
    end,
    function(act, dt)
      act:Done()
    end,
    nil,
    true))

  self.List:PushBack(Action(
    Timed(1),
    function(act, dt)
      MoveText(self, Vec3(0, 0, 800), Vec3(0, 0, 0))(act, dt)
      FadeIn(self)(act, dt)
    end,
    nil,
    true))

  self.List:PushBack(Action(
    nil,
    function(act)
      local obj = self._spacePtr:CreateObject("LeftIntroP")
      obj.Transform:SetTranslation(Vec3(-980, 0, 0))
      obj = self._spacePtr:CreateObject("RightIntroP")
      obj.Transform:SetTranslation(Vec3(980, 0, 0))
      -- obj = self._spacePtr:CreateObject("IntroMachine2")
      -- obj.Transform:SetTranslation(Vec3(0, -830, 0))
      -- self._slotMachine[2] = obj
      act:Done()
    end,
    nil,
    false))

  self.List:PushBack(Action(
    Timed(1.3),
    Hold(),
    nil,
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
    nil,
    function(act)
      local obj = self._spacePtr:CreateObject("TeamLogo")
      obj.Transform:SetTranslation(Vec3(800, -395, 800))
      self._logo = obj
      act:Done()
    end,
    nil,
    false))

  self.List:PushBack(Action(
    Timed(1.5),
    function(act, dt)
      Translate(self, self._logo,
       Vec3(9000, -9000, 800), Vec3(800, -395, 0))(act, dt)
      --MoveObject(self, 2, Vec3(0, -830, 0), Vec3(0, -330, 0))(act, dt)
      Hold()(act, dt)
    end,
    nil,
    false))

  self.List:PushBack(Action(
    Timed(0.6),
    function(act, dt)
      Translate(self, self._slotMachine[1],
       Vec3(-20, 830, 0), Vec3(-20, 290, 0))(act, dt)

      --MoveObject(self, 2, Vec3(0, -830, 0), Vec3(0, -330, 0))(act, dt)
      Hold()(act, dt)
    end,
    nil,
    true))

  self.List:PushBack(Action(
    Timed(0.7),
    FadeOut(self),
    function(act)
      ChangeTexture(self, 4)(act)
      ChangeDimensions(self, Vec2(0.95, 0.95))(act)
    end,
    true))

  self.List:PushBack(Action(
    nil,
    function(act)
      local obj = self._spacePtr:CreateObject("StartBtn")
      self._startBtn = obj
      act:Done()
    end,
    nil,
    false))

  self.List:PushBack(Action(
    Timed(1.0),
    function(act, dt)
      Hold()(act, dt)
      Translate(self, self._startBtn, Vec3(0, -300, 800), Vec3(0, -275, 0))(act, dt)
      --MoveObject(self, 2, Vec3(0, -830, 0), Vec3(0, -330, 0))(act, dt)
      
    end,
    nil,
    false))

  self.List:PushBack(Action(
    Timed(1.5),
    FadeIn(self),
    nil,
    true))

  local funcs = {}

  local function DownAction()
    return Action(Timed(1.75),
      function(act, dt)
        Translate(self, self._startBtn, Vec3(0, -275, 0),
          Vec3(0, -260, 0))(act, dt)
        Hold()(act, dt)
      end,
      function(act)
        self.List:PushBack(funcs[2]())
      end,
      true)
  end

  local function UpAction()
    return Action(Timed(1.75),
      function(act, dt)
        Translate(self, self._startBtn, Vec3(0, -260, 0),
          Vec3(0, -275, 0))(act, dt)
        Hold()(act, dt)
      end,
      function(act)
        self.List:PushBack(funcs[1]())
      end,
      true)
  end

  funcs = {DownAction, UpAction}

  self.List:PushBack(DownAction())


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

  if gamepad.ButtonPressed(nil, GAMEPAD_START) or KeyIsPressed(KEY_ENTER)
  or KeyIsPressed(KEY_SPACE) or KeyIsPressed(KEY_ESCAPE) or
  gamepad.ButtonPressed(nil, GAMEPAD_A) or
  MouseIsDown(MOUSE_LEFT) or MouseIsDown(MOUSE_RIGHT)
  then
    self.List:Clear()

    self.List:PushBack(Action(
    Timed(0.5),
    FadeIn2(self),
    nil,
    true))

    self.List:PushBack(Action(
    function(act)
      engine.ChangeLevel("MainMenu")
    end,
    FadeIn(self),
    nil,
    true))

    
  end
end

function META:Draw()
  surface.SetTexture(self._texID)
  surface.SetColor(255, 255, 255, self._alpha)
  surface.ForceZ(true, self._textPos.z - 1.0)
  surface.DrawTexturedRect(self._textPos.x, self._textPos.z, self._texSize.x, self._texSize.y)
  surface.ForceZ(false, 0)

  if self._alpha2 > 0 then
    surface.SetTexture(self._texID)
    surface.SetColor(0, 0, 0, self._alpha2)
    surface.ForceZ(true, -1.0)
    surface.DrawRect(0, 0, 1920, 1080)
    surface.ForceZ(false, 0)
  end
end

RegisterComponent(META)