local META = GetMeta("Transform")


function META:SetPos(pos)
  self:Settranslation(pos)
end

function META:GetPos()
  return self:Gettranslation()
end