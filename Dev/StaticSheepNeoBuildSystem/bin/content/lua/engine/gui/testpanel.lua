local META = GetMeta("TestPanel")

function META:Init()

end

function META:Think()

end

function META:Paint()
  -- Do the painting yeah
  surface.SetUseCamera(false)
   local tex = surface.GetTextureID("gui/boop.png")
   surface.SetTexture(tex)
   surface.SetRotation(5)
   surface.DrawTexturedRect(60, 50, 32, 32)
  --draw.RoundedBox(32, 10, 10, 250, 250, Color(255, 255, 255, 255))
  surface.SetUseCamera(true)
end

gui.Register( "TestPanel", META, "Panel" )