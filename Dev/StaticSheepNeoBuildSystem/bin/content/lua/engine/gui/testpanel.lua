local META = GetMeta("TestPanel")

function META:Init()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
end

gui.Register( "TestPanel", META, "Panel" )