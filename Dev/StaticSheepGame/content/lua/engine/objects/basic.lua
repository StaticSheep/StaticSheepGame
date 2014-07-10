local META = GetMeta("BasicObject")

function META:Init()
  self:SetupHooks()
end

function META:SetupHooks()
  hook.Add("LogicUpdate", self, self.Update)
end

function META:Update(deltaTime)
end