local META = GetMeta("LuaComponent")

function META:CanUpdate(deltatime)
  if self._space ~= nil then
    if GameSpaces[self._space] ~= nil then
      if GameSpaces[self._space]._paused then
        return false
      else
        return true
      end
    end
  end

  return true
end

function META:Owner()
  return GameSpaces[self._space]._pointer:GetObject(self._owner)
end

function META:Remove()

end

function META:Init()
  self:SetupHooks()
end