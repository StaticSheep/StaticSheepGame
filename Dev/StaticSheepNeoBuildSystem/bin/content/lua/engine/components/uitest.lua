local META = GetMeta("UITest")

InheritMeta(META, "LuaComponent")

function META:Init()
  --print("flanya west init yo SPACE: "..self._space.." CID="..self._cid.." TABLE="..tostring(self))
  self.MyPanel = gui.Create("TestPanel")
  self.super.Init(self)
end

function META:SetupHooks()
  hook.Add("LogicUpdate", self, self.Update, self.CanUpdate)
end

function META:Update(deltaTime)
  --print(self.boomboom)
  --print("UPDATE")
  --print("Update! SPACE: "..self._space.." CID="..self._cid)
end

RegisterComponent(META)