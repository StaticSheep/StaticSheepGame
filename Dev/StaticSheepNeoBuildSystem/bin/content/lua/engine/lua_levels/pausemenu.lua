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

  btn:SetPos(self.base:GetSize().x / 2 - btn:GetSize().x / 2,
    self.ypos)

  self.ypos = self.ypos + ScreenScale(18)

  return btn
end

function PauseMenu:MakeMenu()
  self:CleanUp()

  self.menu = CreateMenu(1, 0, {0, 1, 2, 3})

  self.base = gui.Create("FlatPane")
  self:Register(self.base)
  self.base:SetColor(Color(20, 20, 20, 200))
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

  self.ypos = title:GetPos().y + title:DrawSize().y

  

  local btn

  btn = self:AddButton("Resume")
  btn = self:AddButton("Options")
  btn = self:AddButton("End Match")

end

function PauseMenu:Update()
  if self.base.pos.x < 0 then
    self.base.pos.x = self.base.pos.x + 20
  end
end

PauseMenu:MakeMenu()

hook.Add("LogicUpdate", PauseMenu, PauseMenu.Update)


