
function surface.SetColor(r, g, b, a)
  local alpha = a
  if alpha == nil then alpha = 255 end

  surface.SetColorEx(r, g, b, alpha)
end