local META = GetMeta("Color")

function Color( r, g, b, a )
  a = a or 255
  return setmetatable( { r = math.min( tonumber(r), 255 ), g =  math.min( tonumber(g), 255 ), b =  math.min( tonumber(b), 255 ), a =  math.min( tonumber(a), 255 ) }, COLOR )
end

function ColorAlpha( c, a )
  return Color( c.r, c.g, c.b, a )
end

function IsColor( obj )
  return getmetatable(obj) == META
end


function META:__tostring()
  return string.format( "%d %d %d %d", self.r, self.g, self.b, self.a )
end

function META:__eq( c )
  return self.r == c.r and self.g == c.g and self.b == c.b and self.a == c.a
end

function META:ToVector( )
  return Vec3( self.r / 255, self.g / 255, self.b / 255 )
end

if not color then color = {} end
function color.Lerp(startCol, endCol, frac)
  local retCol = Color(255, 0, 255)

  retCol.r = startCol.r + (endCol.r - startCol.r) * frac
  retCol.g = startCol.g + (endCol.g - startCol.g) * frac
  retCol.b = startCol.b + (endCol.b - startCol.b) * frac

  return retCol
end