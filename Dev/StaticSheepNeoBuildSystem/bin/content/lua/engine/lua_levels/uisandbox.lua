if not Sandbox then
  Sandbox = {}
  Sandbox.panels = {}
  Sandbox.menu = nil
end

function Sandbox:CleanUp()
  for k,v in pairs(self.panels) do
    v:Destroy()
  end

  self.panels = {}

  if self.menu ~= nil then
    self.menu:Destroy()
    self.menu = nil
  end
end

function Sandbox:Register(pnl)
  table.insert(self.panels, pnl)
end


local bodyFiles = {"ninjaBody.png", "ruiserBody.png",
 "europeBody.png", "americaBody.png"}

local bodySizes = {Vec2(416, 716), Vec2(606, 736),
 Vec2(459, 736), Vec2(416, 736)}


function Sandbox:Create()
  self:CleanUp()

  local playerID = 1

  local base = gui.Create("Frame")
  self.base = base
  self:Register(self.base)
  self.base:SetPriority(-1)
  self.base:SetColor(Color(1, 1, 1, 223))
  self.base:SetBGColor(Color(100, 100, 100, 0))
  self.base:SetBorder(0)
  self.base:SetSize(ScrW(), ScrH())
  self.base:SetPos(0, 0)

  local border = 80

  local winLabel = gui.Create("Label", base)
  self:Register(winLabel)
  winLabel:SetText("WINNER!")
  winLabel:SetSize(158)
  winLabel:SetPos(base:GetSize().x / 2, 20)
  winLabel:SetXAlign(TEXT_ALIGN_CENTER)
  winLabel:SetColor(Color(255, 255, 255, 255))

  local winner = 4

  local char = gui.Create("Image", base)
  self:Register(char)
  char:SetTexture(bodyFiles[1])
  if winner == 1 then
    char:SetSize(300, 600)
    char:SetPos(50, 200)
  else
    char:SetSize(300 * 0.7, 600 * 0.7)
    char:SetPos(50, 240)
    char:SetColor(Color(100, 100, 100))
  end
  

  char = gui.Create("Image", base)
  self:Register(char)
  char:SetTexture(bodyFiles[2])

  if winner == 2 then
    char:SetSize(450, 550)
    char:SetPos(ScrW() / 4 - 100, 200)
  else
    char:SetSize(450 * 0.7, 550 * 0.7)
    char:SetPos(ScrW() / 4, 240)
    char:SetColor(Color(100, 100, 100))
  end

  char = gui.Create("Image", base)
  self:Register(char)
  char:SetTexture(bodyFiles[3])

  if winner == 3 then
    char:SetSize(350, 550)
    char:SetPos(ScrW() / 2, 200)
  else
    char:SetSize(350 * 0.7, 550 * 0.7)
    char:SetPos(ScrW() / 2, 240)
    char:SetColor(Color(100, 100, 100))
  end

  char = gui.Create("Image", base)
  self:Register(char)
  char:SetTexture(bodyFiles[4])

  if winner == 4 then
    char:SetSize(350, 550)
    char:SetPos(ScrW() * 3 / 4, 200)
  else
    char:SetSize(350 * 0.7, 550 * 0.7)
    char:SetPos(ScrW() * 3 / 4, 240)
    char:SetColor(Color(100, 100, 100))
  end
  

  local light = gui.Create("Image", base)
  self:Register(light)
  light:SetTexture("spotlight.png")
  light:SetColor(Color(255, 255, 200, 200))
  light:SetSize(400, base:GetSize().y - 100)
  if winner == 1 then
    light:SetPos(0, 0, 1)
  elseif winner == 2 then
    light:SetPos(ScrW() / 4 - 260, 0, 1)
    light:SetSize(800, base:GetSize().y - 100)
  elseif winner == 3 then
    light:SetPos(ScrW() / 2 - 30, 0, 1)
  else
    light:SetPos(ScrW() * 3 / 4 - 20, 0, 1)
  end

  local winnerLabel = gui.Create("Label", base)
  self:Register(winnerLabel)
  winnerLabel:SetText("Press START to continue")
  winnerLabel:SetPos(ScrW() / 2, ScrH() - 20)
  winnerLabel:SetXAlign(TEXT_ALIGN_CENTER)
  winnerLabel:SetYAlign(TEXT_ALIGN_BOTTOM)
  winnerLabel:SetSize(64)
  winnerLabel:SetColor(Color(255, 255, 255))

end

function Sandbox:Update()

end

--Sandbox:Create()