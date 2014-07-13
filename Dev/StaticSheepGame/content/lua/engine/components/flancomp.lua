local META = GetMeta("Flancomp")

InheritMeta(META, "LuaComponent")

function META:Init()
  --print("flanya west init yo SPACE: "..self._space.." CID="..self._cid.." TABLE="..tostring(self))

  self.testval1 = 10
  self.boomboom = 20
  self.bang = false
  self.boom = "wow"

  

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