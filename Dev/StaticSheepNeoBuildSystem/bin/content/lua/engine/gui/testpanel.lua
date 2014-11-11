local META = GetMeta("TestPanel")

function META:Init()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
  surface.DrawRect(0,0,100,100)
end

gui.Register( "TestPanel", META, "Panel" )