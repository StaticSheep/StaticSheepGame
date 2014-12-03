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

local function HealthPaint(self)
  local pos = self:DrawPos()
  local highHP = Color(0, 255, 0)
  local midHP = Color(250, 200, 0)
  local lowHP = Color(255, 0, 0)

  local barColor

  local size = (self:GetSize().x / 10) - (self:GetSize().x / 40)
  for i=0,9 do
    if self.HP > i * 10 then
      if i < 5 then
        barColor = color.Lerp(lowHP, midHP, i / 4)
      else
        barColor = color.Lerp(midHP, highHP, (i - 5) / 5)
      end
      draw.RoundedBox(2, pos.x + i * size + (self:GetSize().x / 40) * (i + 0.5), pos.y, size, 20, barColor)
    end
  end


  
end

local headFiles = {"ninja_head.png", "ruiser_head.png", "europe_head.png", "america_head.png"}
local headColors = {Color(0, 255, 0), Color(255, 0, 0), Color(200, 0, 200), Color(0, 120, 255)}

function Sandbox:Create()
  self:CleanUp()

  local playerID = 1

  self.base = gui.Create("Frame")
  self:Register(self.base)
  self.base:SetColor(Color(50, 50, 50, 0))
  self.base:SetBGColor(Color(100, 100, 100, 0))
  self.base:SetBorder(2)
  self.base:SetSize(150, 150)
  self.base:SetPos(ScrW() / 2, 70)
  self.base:SetCamMode(2)

  local head = gui.Create("Image", self.base)
  self:Register(head)
  head:SetTexture(headFiles[playerID])
  head:SetSize(80, 80)
  head:SetColor(headColors[playerID])

  local lives = gui.Create("Label", self.base)
  self:Register(lives)
  lives:SetSize(32)
  lives:SetText("x 3")
  lives:SetPos(80, 80)
  lives:SetYAlignment(TEXT_ALIGN_BOTTOM)

  local healthbar = gui.Create("Panel", self.base)
  self:Register(healthbar)
  healthbar:SetPos(5, 80 + 5)
  healthbar:SetSize(self.base:GetSize().x - 10, 30)
  healthbar.Paint = HealthPaint
  healthbar.HP = 100

  -- local np = gui.Create("Frame", self.base)
  -- self:Register(np)
  -- np:SetBorder(2)
  -- np:SetSize(self.base:GetSize().x, 30)
  -- np:SetColor(Color(240, 0, 0, 255))

  -- local p = gui.Create("Label", np)
  -- self:Register(p)
  -- p:SetPos(np:GetSize().x / 2, 0)
  -- p:SetColor(Color(255, 255, 255))
  -- p:SetSize(24)
  -- p:SetFont("Consolas")
  -- p:SetText("P1")
  -- p:SetXAlignment(TEXT_ALIGN_CENTER)

  -- p = gui.Create("Panel", self.base)
  -- self:Register(p)
  -- p:SetPos(self.base.border, np:GetSize().y)
  -- p:SetSize(self.base:GetSize().x - self.base.border * 2, self.base:GetSize().y - np:GetSize().y - self.base.border)
  -- p.Paint = HealthPaint
  -- p.texture = surface.GetTextureID("america_head.png")
  -- p.lifes = 12

end

function Sandbox:Update()
end

--Sandbox:Create()
