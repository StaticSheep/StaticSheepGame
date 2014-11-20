
if not TopMenu then
  TopMenu = {}
  TopMenu.panels = {}
end

local function CleanUp()
  for k,v in pairs(TopMenu.panels) do
    v:Destroy()
  end

  TopMenu.panels = {}
end

local function Register(pnl)
  TopMenu.panels[#TopMenu.panels + 1] = pnl
end

local function MakeMenu()
  CleanUp()

  local base = gui.Create("Panel")
  Register(base)
  base:SetSize(ScrW() / 2, ScrH())
  base:SetPos(ScrW() / 4, 0)

  local title = gui.Create("Label", base)
  Register(title)
  title:SetPos(60, 40)
  title:SetText("GIGA GRAVITY GAMES")
  title:SetFontSize(86)
  title:SetFont("Arial")


end


MakeMenu()