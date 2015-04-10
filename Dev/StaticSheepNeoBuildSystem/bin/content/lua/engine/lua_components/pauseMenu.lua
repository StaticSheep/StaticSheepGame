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
  if self.optionsMenu ~= nil then
    self.optionsMenu:Destroy()
    self.optionsMenu = nil
  end


  if self.menu ~= nil then
    self.menu:Destroy()
    self.menu = nil
  end

  if self.confirmMenu ~= nil then
    self.confirmMenu:Destroy()
    self.confirmMenu = nil
  end

  self.base = nil
  self.optionsPanel = nil
  self.confirmPanel = nil
  self.helpMenu = nil
  self._opened = false

  self:HideConfirm()
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
  self.confirmBtns = {}
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

  btn = self:AddButton("Options")
  btn:SetOnPressed(function()
    self:MakeOptionsMenu()
  end)

  btn = self:AddButton("Credits")
  btn:SetOnPressed(function()
     self:Confirm("  Credits will end\nthe current game.",
      "Yes", function(bself)
      self:CleanUp()
      audio.PauseAll(false)
      engine.ChangeLevel("Credits")
      end)
  end)

  local btnb = self:AddButton("Main Menu")
  btnb.firstPress = true
  btnb:SetOnPressed(function()
    self:Confirm("Return to main menu?",
      "Yes", function(bself)
      audio.PauseAll(false)
      self:CleanUp()
      engine.ChangeLevel("MainMenu")
      end)
  end)

  btn = self:AddButton("Quit Game")
  btn.firstPress = true
  btn:SetOnPressed(function()
    self:Confirm("Quit the game?",
      "Yes", function(bself)
      engine.Quit()
      end)
  end)

end

function PauseMenu:AddConfirmButton(text)
  local btn = gui.Create("Button", self.confirmPanel)
  table.insert(self.confirmBtns, btn)
  self.confirmMenu:Add(btn)

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

  btn:SetPos(self.confirmPanel:GetSize().x / 2 - btn:GetSize().x / 2,
    self.ypos)

  self.ypos = self.ypos + 48

  return btn
end

function PauseMenu:HideConfirm()
  if self.confirmMenu ~= nil then
    self.confirmMenu:Destroy()
    self.confirmMenu = nil
  end

  if self.confirmBtns then
    for k,v in pairs(self.confirmBtns) do
      v:Destroy()
    end

    self.confirmBtns = {}
  end

  
  self.confirmPanel = nil
end

function PauseMenu:Confirm(text, confirmText, result)
  if not self.base then return end
  self:HideHelpMenu()
  self:HideOptionsMenu()

  

  self.menu:SetActive(false)

  self.confirmMenu = CreateMenu(1, 0, {0, 1, 2, 3})
  self.confirmPanel = gui.Create("FlatPane")
  table.insert(self.confirmBtns, self.confirmPanel)
  self.confirmPanel:SetColor(Color(0, 0, 0, 220))
  self.confirmPanel:SetDSize(640 / 4, 640)
  self.confirmPanel:SetPos(self.base.size.x, 0, -1)
  self.confirmPanel:SetCamMode(2)

  local title = gui.Create("Label", self.confirmPanel)
  table.insert(self.confirmBtns, title)
  title:SetPos(self.base:GetSize().x / 2, 640 / 5)
  title:SetText("Confirm")
  title:SetDynamicSizing(true)
  title:SetFontSize(22)
  title:SetFont("aircruiser")
  title:SetXAlignment(TEXT_ALIGN_CENTER)
  title:SetColor(Color(255, 255, 255))

  self.ypos = title:GetPos().y + 50

  local warning = gui.Create("Label", self.confirmPanel)
  table.insert(self.confirmBtns, warning)
  warning:SetPos(self.base:GetSize().x / 2, self.ypos)
  warning:SetText(text)
  warning:SetDynamicSizing(true)
  warning:SetFontSize(8)
  warning:SetFont("aircruiser")
  warning:SetXAlignment(TEXT_ALIGN_CENTER)
  warning:SetColor(Color(255, 255, 255))

  self.ypos = self.ypos + 50

  local btn
  btn = self:AddConfirmButton("Confirm")
  table.insert(self.confirmBtns, btn)
  btn:SetOnPressed(function(bself)
    result(bself)
  end)

  local btn
  btn = self:AddConfirmButton("Cancel")
  table.insert(self.confirmBtns, btn)
  btn:SetOnPressed(function(bself)
    self.menu:SetActive(true)
    self:HideConfirm()
  end)
end


function PauseMenu:AddOptionsButton(text)
  local btn = gui.Create("Button", self.optionsPanel)
  self:Register(btn)
  self.optionsMenu:Add(btn)

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

  btn:SetPos(self.optionsPanel:GetSize().x / 2 - btn:GetSize().x / 2,
    self.ypos)

  self.ypos = self.ypos + 48

  return btn
end


local function UpdateMusicButton(btn, vol)

  if vol then
    engine.SetMusicVolume(vol)
  end

  local level = round(engine.GetMusicVolume(), 1)
  btn.level = level
  level = level * 100

  btn:SetText("Music: "..level.."%")
end

local function UpdateSFXButton(btn, vol)

  if vol then
    engine.SetSFXVolume(vol)
  end

  local level = round(engine.GetSFXVolume(), 1)
  btn.level = level
  level = level * 100

  btn:SetText("SFX: "..level.."%")
end

local function UpdateMasterButton(btn, vol)

  if vol then
    engine.SetMasterVolume(vol)
  end

  local level = round(engine.GetMasterVolume(), 1)
  btn.level = level
  level = level * 100

  btn:SetText("Master: "..level.."%")
end

function PauseMenu:MakeOptionsMenu()
  if not self.base then return end
  self:HideHelpMenu()

  self.menu:SetActive(false)

  if self.optionsMenu then
    self.optionsPanel.visible = true
    self.optionsMenu:SetActive(true)
    self.optionsMenu:Select(1)
    return
  end

  self.optionsMenu = CreateMenu(1, 0, {0, 1, 2, 3})
  self.optionsPanel = gui.Create("FlatPane")
  self:Register(self.optionsPanel)
  self.optionsPanel:SetColor(Color(0, 0, 0, 220))
  self.optionsPanel:SetDSize(640 / 4, 640)
  self.optionsPanel:SetPos(self.base.size.x, 0, -1)
  self.optionsPanel:SetCamMode(2)

  local title = gui.Create("Label", self.optionsPanel)
  self:Register(title)
  title:SetPos(self.base:GetSize().x / 2, 640 / 5)
  title:SetText("Options")
  title:SetDynamicSizing(true)
  title:SetFontSize(22)
  title:SetFont("aircruiser")
  title:SetXAlignment(TEXT_ALIGN_CENTER)
  title:SetColor(Color(255, 255, 255))

  self.ypos = title:GetPos().y + 100

  local btn
  btn = self:AddOptionsButton("Master: On")
  self:Register(btn)
  UpdateMasterButton(btn)
  btn:SetOnPressed(function(bself)
    bself.level = bself.level + 0.1
    if bself.level > 1.0 then
      bself.level = 0.0
    end
    UpdateMasterButton(bself, bself.level)
  end)

  local btn
  btn = self:AddOptionsButton("Music: On")
  self:Register(btn)
  UpdateMusicButton(btn)

  btn:SetOnPressed(function(bself)
    bself.level = bself.level + 0.1
    if bself.level > 1.0 then
      bself.level = 0.0
    end
    UpdateMusicButton(bself, bself.level)
  end)

  local btn
  btn = self:AddOptionsButton("SFX: On")
  self:Register(btn)
  UpdateSFXButton(btn)
  btn:SetOnPressed(function(bself)
    bself.level = bself.level + 0.1
    if bself.level > 1.0 then
      bself.level = 0.0
    end
    UpdateSFXButton(bself, bself.level)
  end)

  btn = self:AddOptionsButton("Fullscreen")
  self:Register(btn)
  btn:SetOnPressed(function()
    surface.SetFullScreen(not surface.GetFullScreen())
    --self:MakeOptionsMenu()
    --self.optionMenu:Select(2)
  end)

  local btn
  btn = self:AddOptionsButton("Back")
  btn:SetOnPressed(function()
    self:HideOptionsMenu()
    self.menu:SetActive(true)
  end)

end

function PauseMenu:HideOptionsMenu()
  if self.optionsMenu then
    --print("Hide options menu yo")
    self.optionsPanel.visible = false
    self.optionsMenu:SetActive(false)
  end
end


function PauseMenu:MakeHelpMenu()
  if not self.base then return end
  if self.helpMenu then return end
  self:HideOptionsMenu()

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
  self:HideOptionsMenu()
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