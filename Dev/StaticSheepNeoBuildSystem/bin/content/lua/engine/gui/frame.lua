local META = GetMeta("Frame")

function META:Init()
end

function META:Think()

end

function META:Paint()
  surface.SetCamState(2)

  local border = 4
  local pos = self:DrawPos()

  draw.RoundedBox(4, pos.x, pos.y,
    self.size.x, self.size.y, self.bgColor)

  draw.RoundedBox(4, border + pos.x, border + pos.y,
    self.size.x - border * 2, self.size.y - border * 2, self.color)

  surface.SetCamState(0)
end

gui.Register( "Frame", META, "Panel" )