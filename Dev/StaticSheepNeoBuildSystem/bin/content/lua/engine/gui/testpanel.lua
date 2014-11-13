local META = GetMeta("TestPanel")

function META:Init()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
  surface.SetColor(255, 0, 0, 255)
  surface.DrawRect(0,0,1100,100)
  surface.SetPos(-300, 0)
  surface.DrawString("Monkey Dickbutt", 80, "Arial")
end

gui.Register( "TestPanel", META, "Panel" )