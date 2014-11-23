local META = GetMeta("FlatPane")

function META:Init()
end

function META:Think()

end

function META:Paint()
  local border = 4

  local pos = self:DrawPos()

  draw.RoundedBox(4, border + pos.x, border + pos.y,
    self.size.x - border * 2, self.size.y - border * 2, self.color)
end

gui.Register( "FlatPane", META, "Panel" )