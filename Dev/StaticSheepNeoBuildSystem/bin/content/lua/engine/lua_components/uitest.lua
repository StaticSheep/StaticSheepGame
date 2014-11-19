local META = GetMeta("UITest")

InheritMeta(META, "LuaComponent")

function META:Init()
  self.MyPanel = gui.Create("Button")

  self.MyPanel.pos = Vec2(100, 100)
  self.MyPanel.size = Vec2(512, 256)

  self.MyPanel:SetColor(Color(180, 180, 180, 255))
  self.MyPanel:SetBGColor(Color(100, 100, 100, 255))

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