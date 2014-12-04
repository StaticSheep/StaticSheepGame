
if not TopMenu then
  TopMenu = {}
  TopMenu.panels = {}
  TopMenu.menu = nil
end

local function CleanUp()
  for k,v in pairs(TopMenu.panels) do
    v:Destroy()
  end

  TopMenu.panels = {}

  if TopMenu.menu ~= nil then
    TopMenu.menu:Destroy()
    TopMenu.menu = nil
  end
end

local function Register(pnl)
  TopMenu.panels[#TopMenu.panels + 1] = pnl
end

local function DoSomething()
  print("loading level")
end

local function MakeMenu()
  CleanUp()

  local base = gui.Create("Frame")
  Register(base)
  base:SetSize(ScrW() / 2, ScrH())
  base:SetPos(ScrW() / 4, 0)
  base:SetCamMode(2)

  local title = gui.Create("Label", base)
  Register(title)
  title:SetPos(base:GetSize().x / 2, 5)
  title:SetText("GIGA GRAVITY GAMES")
  title:SetFontSize(ScreenScale(24))
  title:SetFont("Arial")
  title:SetXAlignment(TEXT_ALIGN_CENTER)

  local menu = CreateMenu(1, 1, {0})
  TopMenu.menu = menu

  local ypos = 30

  local btn = gui.Create("Button", base)
  Register(btn)
  menu:Add(btn)
  btn:SetText("Play")
  btn:SetFontSize(ScreenScale(12))
  btn:SetSize(200, ScreenScale(18))
  btn:SetPos(base:GetSize().x / 2 - btn:GetSize().x / 2,
    ScreenScale(ypos))
  btn:SetOnPressed(DoSomething)

  ypos = ypos + 20

  btn = gui.Create("Button", base)
  Register(btn)
  menu:Add(btn)
  btn:SetFontSize(ScreenScale(12))
  btn:SetText("Load")
  btn:SetSize(200, ScreenScale(18))
  btn:SetPos(base:GetSize().x / 2 - btn:GetSize().x / 2,
   ScreenScale(ypos))
  ypos = ypos + 20

  btn = gui.Create("Button", base)
  Register(btn)
  menu:Add(btn)
  btn:SetFontSize(ScreenScale(12))
  btn:SetText("Report me")
  btn:SetSize(200, ScreenScale(18))
  btn:SetPos(base:GetSize().x / 2 - btn:GetSize().x / 2,
   ScreenScale(ypos))
  ypos = ypos + 20


  print(GamePad(0):Connected())

end


MakeMenu()