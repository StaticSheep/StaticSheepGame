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

function META:Init()
  self.super.Init(self)

  self.tex = {}
  self.tex[1] = surface.GetTextureID("InTheFuture.png")
  self.tex[2] = surface.GetTextureID("ThereIsOnly.png")
  self.tex[3] = surface.GetTextureID("Entertainment.png")

  self._alpha = 255

  self.List = actionlist.Create()
  self.List:PushBack(Action(
    function(act)
      act.tl = 2
      act.ttl = 2
    end,
    function(act, dt)
      act.tl = act.tl - dt
      self._alpha = lerp(0, 255, (1-(act.tl/act.ttl)))
      --print(tostring(self._alpha))
      if act.tl < 0 then
        act:Done()
      end
    end,
    nil,
    true))
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
  surface.SetTexture(self.tex[1])
  surface.SetColor(255, 255, 255, self._alpha)
  surface.DrawTexturedRect(0, 0, 1, 1)
end

RegisterComponent(META)