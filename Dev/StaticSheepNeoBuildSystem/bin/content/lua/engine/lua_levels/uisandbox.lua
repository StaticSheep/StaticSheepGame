--[[*****************************************************************
Filename: uisandbox.lua
Project: Giga Gravity Games
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************]]
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




function Sandbox:Create()
  self:CleanUp()


end

function Sandbox:Update()

end

Sandbox:Create()