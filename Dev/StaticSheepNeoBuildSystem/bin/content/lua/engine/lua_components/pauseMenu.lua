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
  self.opened = false
end

function PauseMenu:Register(pnl)
  table.insert(self.panels, pnl)
end

function PauseMenu:AddButton(text)
  local btn = gui.Create("Button", self.base)
  self:Register(btn)
  self.menu:Add(btn)

  btn:SetText(text)

  btn:SetFontSize(ScreenScale(12))
  btn:SetSize(ScreenScale(80), ScreenScale(16))

  btn:SetFontColor(Color(200, 200, 200, 255))
  btn:SetColor(Color(20, 20, 20, 0))
  btn:SetBGColor(Color(20, 20, 20, 0))

  btn:SetHoverColor(Color(100, 100, 100, 0))
  btn:SetHoverBGColor(Color(30, 30, 30, 0))
  btn:SetHoverFontColor(Color(255, 220, 130, 255))

  btn:SetClickColor(Color(100, 100, 100, 0))
  btn:SetClickBGColor(Color(20, 20, 20, 50))
  btn:SetClickFontColor(Color(255, 255, 120, 255))

  btn:SetPos(self.base:GetSize().x / 2 - btn:GetSize().x / 2,
    self.ypos)

  self.ypos = self.ypos + ScreenScale(18)

  return btn
end

function PauseMenu:MakeMenu()
  self.opened = true
  self.ypos = 0

  self.menu = CreateMenu(1, 0, {0, 1, 2, 3})

  self.base = gui.Create("FlatPane")
  self:Register(self.base)
  self.base:SetColor(Color(0, 0, 0, 220))
  self.base:SetSize(ScrW() / 4, ScrH())
  self.base:SetPos(-self.base.size.x, 0)
  self.base:SetCamMode(2)

  local title = gui.Create("Label", self.base)
  self:Register(title)
  title:SetPos(self.base:GetSize().x / 2, ScrH() / 4)
  title:SetText("PAUSE")
  title:SetFontSize(ScreenScale(32))
  title:SetFont("Consolas")
  title:SetXAlignment(TEXT_ALIGN_CENTER)
  title:SetColor(Color(255, 255, 255))

  self.ypos = title:GetPos().y + title:DrawSize().y

  local btn
  btn = self:AddButton("Resume")
  btn:SetOnPressed(function()
    self:CloseMenu()
  end)

  btn = self:AddButton("How to Play")
  btn:SetOnPressed(function()
    self:MakeHelpMenu()
  end)

  btn = self:AddButton("Quit Game")
  btn.firstPress = true
  btn:SetOnPressed(function()
    if btn.firstPress then
      btn.firstPress = false
      btn:SetText("Quit Game [CONFIRM]")
    else
      print("QUIT THE GAME")
    end
  end)

  local rights = gui.Create("Label", self.base)
  self:Register(rights)
  rights:SetPos(self.base:GetSize().x / 2, ScrH() - 100)
  rights:SetText("All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.\nFMOD Studio Copyright (c) 2005-2011 Firelight Technologies Pty, Ltd.")
  rights:SetXAlignment(TEXT_ALIGN_CENTER)
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
  helpImage:SetTexture("Controller_Instructions2.png")

  local helpText = gui.Create("Label", helpMenu)
  self:Register(helpText)
  helpText:SetText("Keyboard Controls:\nW,A,S,D - Menu Navigation\nENTER - Menu Selection")
  helpText:SetPos(helpMenu:GetSize().x * 0.2, helpMenu:GetSize().y * 0.6 + helpMenu:GetSize().y * 0.1 + 10)
  helpText:SetSize(24)
end

function PauseMenu:HideHelpMenu()
  if self.helpMenu then
    self.helpMenu.visible = false
  end
end

function PauseMenu:CloseMenu()
  self:HideHelpMenu()
  self.opened = false
  self.menu:SetActive(false)
end

function PauseMenu:Init()
  self.panels = {}
  self.menu = nil
  self.opened = false

  self.super.Init(self)
end

function PauseMenu:Refresh()
  self:CleanUp()
  self:PauseSpace(false)

  self:PauseSpace(true)
  self:MakeMenu()
  --self:MakeMenu()
end

function PauseMenu:Remove()
  self:CleanUp()
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
  if not self.opened and not self.base then
    if KeyIsPressed(KEY_ESCAPE) or
      gamepad.ButtonPressed(nil, GAMEPAD_START) then

      Log(DEBUG, "Pausing space")
      self:PauseSpace(true)

      self:MakeMenu()
    end
  end
end

function PauseMenu:FrameUpdate(deltaTime)
  if self.opened then
    if self.base.pos.x < 0 then
      self.base.pos.x = self.base.pos.x + 20
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