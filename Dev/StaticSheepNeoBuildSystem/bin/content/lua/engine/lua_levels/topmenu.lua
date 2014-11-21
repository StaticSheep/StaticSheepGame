
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

local function MakeMenu()
  CleanUp()

  local base = gui.Create("Frame")
  Register(base)
  base:SetSize(ScrW() / 2, ScrH())
  base:SetPos(ScrW() / 4, 0)
  base:SetCamMode(2)

  local title = gui.Create("Label", base)
  Register(title)
  title:SetPos(base:GetSize().x / 2, 0)
  title:SetText("GIGA GRAVITY GAMES")
  title:SetFontSize(48)
  title:SetFont("Arial")
  title:SetXAlignment(TEXT_ALIGN_CENTER)

  local menu = CreateMenu(1, 1, {0})
  TopMenu.menu = menu

  local btn = gui.Create("Button", base)
  Register(btn)
  menu:Add(btn)
  btn:SetSize(200, 50)
  btn:SetPos(base:GetSize().x / 2 - btn:GetSize().x / 2, 100)

  btn = gui.Create("Button", base)
  Register(btn)
  menu:Add(btn)
  btn:SetSize(200, 50)
  btn:SetPos(base:GetSize().x / 2 - btn:GetSize().x / 2, 200)

  btn = gui.Create("Button", base)
  Register(btn)
  menu:Add(btn)
  btn:SetSize(200, 50)
  btn:SetPos(base:GetSize().x / 2 - btn:GetSize().x / 2, 300)

  btn = gui.Create("Button", base)
  Register(btn)
  menu:Add(btn)
  btn:SetSize(200, 50)
  btn:SetPos(base:GetSize().x / 2 - btn:GetSize().x / 2, 400)

  print(GamePad(0):Connected())

end


MakeMenu()