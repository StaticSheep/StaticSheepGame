local META = GetMeta("FlatPane")

function META:Init()
end

function META:Think()

end

function META:Paint()
  local pos = self:DrawPos()

  draw.RoundedBox(0, pos.x, pos.y,
    self.size.x, self.size.y, self.color)
end

gui.Register( "FlatPane", META, "Panel" )