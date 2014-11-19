local META = GetMeta("Button")

function META:Init()
  PrintTable(GetMeta("Button"))
end

function META:Think()

end

function META:Paint()

end

gui.Register( "Button", META, "Frame" )