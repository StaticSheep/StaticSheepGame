local META = GetMeta("TestPanel")

function META:Init()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
  surface.SetUseCamera(false)
  -- local tex = surface.GetTextureID("ruiserjump.png")
  -- surface.SetTexture(tex)
  -- surface.SetRotation(0)
  -- surface.DrawTexturedRect(50, 50, 150, 150)
  draw.RoundedBox(32, 10, 10, 250, 250, Color(255, 255, 255, 255))
  surface.SetUseCamera(true)
end

gui.Register( "TestPanel", META, "Panel" )