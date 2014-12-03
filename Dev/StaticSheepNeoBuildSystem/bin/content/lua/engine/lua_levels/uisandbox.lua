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


local headFiles = {"ninja_head.png", "ruiser_head.png", "europe_head.png", "america_head.png"}
local headColors = {Color(0, 255, 0), Color(255, 0, 0), Color(200, 0, 200), Color(0, 120, 255)}

function Sandbox:Create()
  self:CleanUp()

  local playerID = 1

  self.base = gui.Create("Frame")
  self:Register(self.base)
  self.base:SetPriority(2)
  self.base:SetColor(Color(1, 1, 1, 223))
  self.base:SetBGColor(Color(100, 100, 100, 0))
  self.base:SetBorder(0)
  self.base:SetSize(ScrW(), ScrH())
  self.base:SetPos(0, 0)
end

function Sandbox:Update()

end

Sandbox:Create()