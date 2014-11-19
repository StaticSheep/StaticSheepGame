
if not surface then surface = {} end

function surface.SetColor(r, g, b, a)
  local alpha = a
  if alpha == nil then alpha = 255 end

  surface.SetColorEx(r / 255, g / 255, b / 255, alpha / 255)
end