--[[*****************************************************************
Filename: pauseMenu.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
local PauseMenu = GetMeta("PauseMenu")

InheritMeta(PauseMenu, "LuaComponent")

function PauseMenu:CleanUp()
  for k,v in pairs(self.panels) do
    v:Destroy()
  end

  self.panels = {}

  if self.menu ~= nil then
    self.menu:Destroy()
    self.menu = nil
  end

  self.base = nil
  self.helpMenu = nil
  self._opened = false
end

function PauseMenu:Register(pnl)
  table.insert(self.panels, pnl)
end

function PauseMenu:AddButton(text)
  local btn = gui.Create("Button", self.base)
  self:Register(btn)
  self.menu:Add(btn)

  btn:SetText(text)
  btn:SetFont("aircruiser")
  btn:SetDynamicSizing(true)

  btn:SetFontSize(12)
  btn:SetSize(80, 16)

  btn:SetYAlignment(TEXT_ALIGN_TOP)

  btn:SetFontColor(Color(255, 255, 255, 255))
  btn:SetColor(Color(20, 20, 20, 0))
  btn:SetBGColor(Color(20, 20, 20, 0))

  btn:SetHoverColor(Color(100, 100, 100, 0))
  btn:SetHoverBGColor(Color(30, 30, 30, 0))
  btn:SetHoverFontColor(Color(200, 200, 120, 255))

  btn:SetClickColor(Color(100, 100, 100, 0))
  btn:SetClickBGColor(Color(20, 20, 20, 0))
  btn:SetClickFontColor(Color(255, 255, 120, 255))

  btn:SetPos(self.base:GetSize().x / 2 - btn:GetSize().x / 2,
    self.ypos)

  self.ypos = self.ypos + 48

  return btn
end

function PauseMenu:MakeMenu()
  self._opened = true
  self.ypos = 0

  self.menu = CreateMenu(1, 0, {0, 1, 2, 3})

  self.base = gui.Create("FlatPane")
  self:Register(self.base)
  self.base:SetColor(Color(0, 0, 0, 220))
  self.base:SetDSize(640 / 4, 640)
  self.base:SetPos(-self.base.size.x, 0, -1)
  self.base:SetCamMode(2)

  local title = gui.Create("Label", self.base)
  self:Register(title)
  title:SetPos(self.base:GetSize().x / 2, 640 / 5)
  title:SetText("PAUSE")
  title:SetDynamicSizing(true)
  title:SetFontSize(22)
  title:SetFont("aircruiser")
  title:SetXAlignment(TEXT_ALIGN_CENTER)
  title:SetColor(Color(255, 255, 255))

  self.ypos = title:GetPos().y + 100

  local btn
  btn = self:AddButton("Resume Game")
  btn:SetOnPressed(function()
    self:CloseMenu()
  end)

  btn = self:AddButton("How to Play")
  btn:SetOnPressed(function()
    self:MakeHelpMenu()
  end)

  local btnb = self:AddButton("Back to Menu")
  btnb.firstPress = true
  btnb:SetOnPressed(function()
    if btnb.firstPress then
      btnb.firstPress = false
      btnb:SetText("Confirm Return")
    else
      engine.ChangeLevel("MainMenu")
    end
  end)

  btn = self:AddButton("Quit Game")
  btn.firstPress = true
  btn:SetOnPressed(function()
    if btn.firstPress then
      btn.firstPress = false
      btn:SetText("Confirm Quit")
    else
      engine.Quit()
    end
  end)

end

function PauseMenu:MakeHelpMenu()
  if not self.base then return end
  if self.helpMenu then return end

  local helpMenu = gui.Create("FlatPane", self.base)
  self:Register(helpMenu)
  helpMenu:SetColor(Color(0, 0, 0, 220))
  helpMenu:SetSize(ScrW() * 3 / 4, ScrH())
  helpMenu:SetPos(ScrW() / 4, 0)

  self.helpMenu = helpMenu

  local helpImage = gui.Create("Image", helpMenu)
  self:Register(helpImage)
  helpImage:SetSize(helpMenu:GetSize().x * 0.6, helpMenu:GetSize().y * 0.6)
  helpImage:SetPos(helpMenu:GetSize().x * 0.2, helpMenu:GetSize().y * 0.1)
  helpImage:SetTexture("Controller_Instructions.png")

  local requires = gui.Create("Label", helpMenu)
  self:Register(requires)
  requires:SetText("This game uses an XBOX game pad.")
  requires:SetPos(helpMenu:GetSize().x / 2, 30)
  requires:SetSize(ScreenScale(5))
  requires:SetColor(Color(255, 20, 20))
  requires:SetXAlignment(TEXT_ALIGN_CENTER)

  -- local gstart = gui.Create("Label", helpMenu)
  -- self:Register(gstart)
  -- gstart:SetText("Press A to start the game.")
  -- gstart:SetPos(helpMenu:GetSize().x / 2, 80)
  -- gstart:SetSize(ScreenScale(8))
  -- gstart:SetColor(Color(255, 255, 255))
  -- gstart:SetXAlignment(TEXT_ALIGN_CENTER)

  local helpText = gui.Create("Label", helpMenu)
  self:Register(helpText)
  helpText:SetText("Keyboard Controls:\nW,A,S,D - Menu Navigation & Movement\nArrow Keys - Aim\nENTER - Menu Selection\nQ - Jump\nSpace - Fire")
  helpText:SetPos(helpMenu:GetSize().x * 0.2, helpMenu:GetSize().y * 0.6 + helpMenu:GetSize().y * 0.1 + 10)
  helpText:SetSize(ScreenScale(5))
end

function PauseMenu:HideHelpMenu()
  if self.helpMenu then
    self.helpMenu.visible = false
  end
end

function PauseMenu:CloseMenu()
  self:HideHelpMenu()
  self._opened = false
  self.menu:SetActive(false)
end

function PauseMenu:Init()
  self.panels = {}
  self.menu = nil
  self._opened = false

  self.super.Init(self)

  -- self:PauseSpace(true)


  -- self:MakeMenu()
  -- self:MakeHelpMenu()

  --self.base.pos.x = 0
end

function PauseMenu:Refresh()
  --self:PauseSpace(true)
  --self:CleanUp()
  
  self:CleanUp()
  self:MakeMenu()
  --self:MakeHelpMenu()

  --self:PauseSpace(true)
  --self:MakeMenu()
  --self:MakeMenu()
end

function PauseMenu:Remove()
  self:CleanUp()
  hook.Remove("FrameUpdate", self)
  hook.Remove("LogicUpdate", self)
end

function PauseMenu:SetupHooks()
  hook.Add("FrameUpdate", self, self.FrameUpdate)
  hook.Add("LogicUpdate", self, self.LogicUpdate, self.CanUpdate)
end

function PauseMenu:PauseSpace(paused)
  audio.PauseAll(paused)
  gamespace.Get(self._space)._pointer:SetPaused(paused)
end

function PauseMenu:LogicUpdate(deltaTime)
  if not self._opened and not self.base then
    if KeyIsPressed(KEY_ESCAPE) or
      gamepad.ButtonPressed(nil, GAMEPAD_START) and
      not GAME_WON then

      Log(DEBUG, "Pausing space")
      self:PauseSpace(true)

      self:MakeMenu()
    end
  end
end

function PauseMenu:FrameUpdate(deltaTime)
  if self._opened then
    if self.base.pos.x < 0 then
      self.base.pos.x = self.base.pos.x + 20
      if (self.base.pos.x > 0) then
        self.base.pos.x = 0
      end
    else
      if KeyIsPressed(KEY_ESCAPE) or
        gamepad.ButtonPressed(nil, GAMEPAD_START) then

        self:CloseMenu()
      end

    end
  else
    if self.base then
      if self.base.pos.x > -self.base.size.x then
        self.base.pos.x = self.base.pos.x - 30
        if not (self.base.pos.x > -self.base.size.x) then
          Log(DEBUG, "Resuming game")

          self:CleanUp()
          self:PauseSpace(false)
        end
      end
    end
  end
end

RegisterComponent(PauseMenu)