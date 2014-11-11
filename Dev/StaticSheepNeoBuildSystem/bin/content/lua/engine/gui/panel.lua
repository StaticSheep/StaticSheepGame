local META = GetMeta("Panel")

function META:Init()
  self.size = Vec2(32, 32)
  self.pos = Vec2(0, 0)
  self.color = Color(100, 100, 100)
  self.visible = true
  self.valid = true
end

function META:Destroy()
  self.valid = false
end

function META:Delete()

end

function META:Prepare()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
end
