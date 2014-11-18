local META = GetMeta("TestPanel")

function META:Init()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
  surface.SetUseCamera(true)
  surface.DrawRect(0, 0, 256, 256)
  draw.RoundedBox(16, 50, 50, 250, 250, Color(255, 255, 255, 255))
  surface.SetUseCamera(true)
end

gui.Register( "TestPanel", META, "Panel" )