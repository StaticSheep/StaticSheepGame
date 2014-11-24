if not PauseMenu then
  PauseMenu = {}
  PauseMenu.panels = {}
  PauseMenu.menu = nil
end

function PauseMenu:CleanUp()
  for k,v in pairs(self.panels) do
    v:Destroy()
  end

  self.panels = {}

  if self.menu ~= nil then
    self.menu:Destroy()
    self.menu = nil
  end
end

function PauseMenu:Register(pnl)
  self.panels[#self.panels + 1] = pnl
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
  self:CleanUp()

  self.opened = true

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
    self.opened = false
    self.menu:SetActive(false)
  end)

  btn = self:AddButton("Options")
  btn:SetOnPressed(function()
    print("Options menu")
  end)

  btn = self:AddButton("End Match")
  btn:SetOnPressed(function()
    print("End the match")

  end)
end

function PauseMenu:Update()

  if self.opened then
    if self.base.pos.x < 0 then
      self.base.pos.x = self.base.pos.x + 20
    end
  else
    if self.base.pos.x > -self.base.size.x then
      self.base.pos.x = self.base.pos.x - 30
      if not (self.base.pos.x > -self.base.size.x) then
        Log(DEBUG, "Resuming game")
      end
    end
  end
end

PauseMenu:MakeMenu()

hook.Add("LogicUpdate", PauseMenu, PauseMenu.Update)


