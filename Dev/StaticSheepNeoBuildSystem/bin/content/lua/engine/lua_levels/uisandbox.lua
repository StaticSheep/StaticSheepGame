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
  surface.SetTexture(self.texture)
  surface.SetColor(255, 0, 0, 255)

  local size = Vec2(18, 18)
  local perRow = math.floor(self.size.x / size.x)
  local rPos = Vec2(0, 0)

  for i=0, self.lifes - 1 do
    rPos.x = pos.x + size.x * (i % perRow)
    --print(rPos.x)
    rPos.y = pos.y + size.y * math.floor(i / perRow)
    --print(rPos.y)
    surface.DrawTexturedRect(rPos.x, rPos.y, size.x, size.y)
  end

  
end

function Sandbox:Create()
  self:CleanUp()

  self.base = gui.Create("Frame")
  self:Register(self.base)
  self.base:SetColor(Color(50, 50, 50, 220))
  self.base:SetBGColor(Color(100, 100, 100, 200))
  self.base:SetBorder(2)
  self.base:SetSize(116, 71)
  self.base:SetPos(ScrW() / 2, 70)
  self.base:SetCamMode(2)

  local np = gui.Create("Frame", self.base)
  self:Register(np)
  np:SetBorder(2)
  np:SetSize(self.base:GetSize().x, 30)
  np:SetColor(Color(240, 0, 0, 255))

  local p = gui.Create("Label", np)
  self:Register(p)
  p:SetPos(np:GetSize().x / 2, 0)
  p:SetColor(Color(255, 255, 255))
  p:SetSize(24)
  p:SetFont("Consolas")
  p:SetText("P1")
  p:SetXAlignment(TEXT_ALIGN_CENTER)

  p = gui.Create("Panel", self.base)
  self:Register(p)
  p:SetPos(self.base.border, np:GetSize().y)
  p:SetSize(self.base:GetSize().x - self.base.border * 2, self.base:GetSize().y - np:GetSize().y - self.base.border)
  p.Paint = HealthPaint
  p.texture = surface.GetTextureID("life.png")
  p.lifes = 12

end

function Sandbox:Update()
end

--Sandbox:Create()
