local META = GetMeta("Flancomp")

function META:Init()
  --print("flanya west init yo SPACE: "..self._space.." CID="..self._cid.." TABLE="..tostring(self))

  self.testval1 = 10
  self.boomboom = 20
  self.bang = false
  self.boom = "wow"

  hook.Add("LogicUpdate", self, self.Update)

  --print(SerializeComponent(self._space, self._owner, self._cid))

end

function META:Update(deltaTime)
  --print("Update! SPACE: "..self._space.." CID="..self._cid)
end