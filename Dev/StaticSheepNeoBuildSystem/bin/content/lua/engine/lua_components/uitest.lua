local META = GetMeta("UITest")

InheritMeta(META, "LuaComponent")

function META:Init()
  self.MyPanel = gui.Create("TestPanel")
  self.super.Init(self)
end

function META:SetupHooks()
  hook.Add("LogicUpdate", self, self.Update, self.CanUpdate)
end

function META:Update(deltaTime)

end

function META:Remove()
  self.MyPanel:Destroy()
end

RegisterComponent(META)