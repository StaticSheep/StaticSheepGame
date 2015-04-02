--[[*****************************************************************
Filename: intro.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local META = GetMeta("CreditsScreen")

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

local function TimedHold(totalTime, holdTime)
  return function(act)
    act.ttl = totalTime
    act.startHold = (totalTime + holdTime) / 2
    act.endHold = (totalTime - holdTime) / 2
    act.tl = totalTime
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

local function TranslateVector(vector, start, finish, t)
  return function(act, dt)
    vector.x = lerp(start.x, finish.x, 1 - t)
    vector.y = lerp(start.y, finish.y, 1 - t)
  end
end


function META:CreditTitle(title, name, delay, hold)
  self:CreditEx(title, delay, nil, 1.5, 26)
  self:CreditEx(name, 2.0)
end

function META:CreditEx(text, delay, time, hold, size, font)
  local creditBlock = {}
  creditBlock._text = string.gsub(text, "\n", "\n    ")
  creditBlock._active = false
  creditBlock._pos = Vec2(0, -1000)

  if not font then
    font = "aircruiser"
  end

  creditBlock._fontID = surface.GetFontID(font)

  if not size then
    size = 15
  end

  creditBlock._size = size

  if not time then
    time = 5
  end

  if not hold then
    hold = 0.0
  end

  if not delay then
    delay = 0
  end

  if delay > 0 then
    self.List:PushBack(Action(
    Timed(delay),
    Hold(),
    nil,
    true))
  end

  table.insert(self.CreditList, creditBlock)
  

  self.List:PushBack(Action(
    function(act)
      TimedHold(time + hold, hold)(act)
      creditBlock._active = true
    end,
    function(act, dt)
      if act.tl < act.startHold then
        if act.tl < act.endHold then
          -- Going away
          local t = act.tl / act.endHold
          TranslateVector(creditBlock._pos, Vec2(320, 320),
            Vec2(320, -60), t)(act, dt)
        else
          -- Holding
        end
      else
        -- Coming up
        local t = (act.tl - act.startHold) / (act.ttl - act.startHold)
        TranslateVector(creditBlock._pos, Vec2(320, 700),
          Vec2(320, 320), t)(act, dt)

      end
      Hold()(act, dt)
    end,
    function(act)
      creditBlock._active = false
    end,
    false))

  
end


function META:Run()
  self.tex = {}
  self.tex[1] = surface.GetTextureID("GigaGravityTitle.png")
  self.CreditList = {}

  self._alpha = 255
  self._texID = self.tex[1]
  self._texSize = Vec2(600, 150)
  self._textPos = Vec3(20, 50, 0)
  self._slotMachine = {}
  self._logo = nil
  self._startBtn = nil

  self.List = actionlist.Create()

  self.List:PushBack(Action(
    Timed(1),
    Hold(),
    nil,
    true))

  self.List:PushBack(Action(
    Timed(1.5),
    FadeOut(self),
    nil,
    true))

  self:CreditTitle("Claude Comair", "DigiPen President", 0)
  self:CreditEx("GAM Instructors", 2.5, nil, 1.5, 26)
  self:CreditEx("Rachel Rutherford", 2.0)
  self:CreditEx("ellen Beeman", 0.5)
  self:CreditEx("Benjamin ellinger", 0.5)


  self:CreditTitle("Greg Walls", "Producer", 3.0)
  self:CreditTitle("Zachary Nawar", "Technical Director", 2.5)
  self:CreditTitle("Jon Sourbeer", "Physics Pinata", 2.5)
  self:CreditTitle("Scott Nelson", "Graphics Programmer", 2.5)
  self:CreditTitle("Zakary Wilson", "Technical Artist", 2.5)
  self:CreditTitle("evan AldErEtE", "Composer", 2.5)

  self:CreditEx("Special Thanks To", 2.5, nil, 0, 26)

  local thanks = {"Cameron Craig", "Jake Ganz",
"Juli Gregg", "Rachel Rutherford", "Sarah McGinley",
"Nathan Carlson", "Jordan Ellis", "Randy Gaul", "Corbin Hart",
"Robert Di Battista", "Joseph Nawar", "Tresillion Dorne",
"Garry Newman", "Starbucks", "Texas Instruments"
}
  for k,v in pairs(thanks) do
    self:CreditEx(v, 0.75)
  end
  

  local ptesters = {"Jay Ackerman",
"Tai Der Hui", "Mickey Tyler", "Daya Singh",
"Scott Smith", "Nathan Mueller", "Chris Hendricks",
"John Lim", "Patrick Michael \"Overhira\" Casey",
"Izzy Abdus-Sabur", "Ilan Keshet", "Eduardo Gorinstein",
"Esteban Maldonado"}

  self:CreditEx("Playtesters", 2.5, nil, 0, 26)

  for k,v in pairs(ptesters) do
    self:CreditEx(v, 0.75)
  end

  self:CreditEx("All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.", 2.5, nil, 0, 10)
  self:CreditEx("FMOD Studio Copyright (c) 2005-2011 Firelight Technologies Pty, Ltd.", 0.5, nil, 0, 10)
  self:CreditEx("AntTweakBar Copyright (c) 2005-2013 Philippe Decaudin.", 0.5, nil, 0, 10)
  self:CreditEx("DirectX 11 TK Copyright (c) 2015 Microsoft Corp", 0.5, nil, 0, 10)

end

function META:Init()
  self.super.Init(self)
  self.CreditList = {}

  self:Run()
end

function META:Refresh()
  self:Run()
end

function META:SetupHooks()
  hook.Add("LogicUpdate", self, self.Update, self.CanUpdate)
  hook.Add("Draw", self, self.Draw)
  hook.Add("GUIDRAW", self, self.GuiDraw)
end

function META:Remove()
  hook.Remove("LogicUpdate", self)
  hook.Remove("Draw", self)
  hook.Remove("GUIDRAW", self)
end

function META:Update(dt)
  self.List:Update(dt)

  if gamepad.ButtonPressed(nil, GAMEPAD_START) or
  gamepad.ButtonPressed(nil, GAMEPAD_A) or
  gamepad.ButtonPressed(nil, GAMEPAD_B) or
  KeyIsPressed(KEY_ESCAPE) or
  KeyIsPressed(KEY_SPACE) or
  KeyIsPressed(KEY_ENTER) then
    engine.ChangeLevel("MainMenu")
  end
end

function META:GuiDraw()
  for k,credit in pairs(self.CreditList) do
    if credit._active then
      surface.SetRotation(0)
      draw.SimpleText(credit._text, credit._fontID,
       ScreenScale(credit._pos.x), ScreenScaleY(credit._pos.y),
       ScreenScale(credit._size), Color(255, 255, 255, 255),
       TEXT_ALIGN_CENTER, TEXT_ALIGN_CENTER)
    end
  end

  surface.SetTexture(self._texID)
  surface.SetColor(255, 255, 255, self._alpha)
  surface.ForceZ(true, self._textPos.z - 1.0)
  surface.DrawTexturedRect(ScreenScale(self._textPos.x), ScreenScaleY(self._textPos.y),
   ScreenScale(self._texSize.x), ScreenScaleY(self._texSize.y))
  surface.ForceZ(false, 0)
end

function META:Draw()
 

  
end

RegisterComponent(META)