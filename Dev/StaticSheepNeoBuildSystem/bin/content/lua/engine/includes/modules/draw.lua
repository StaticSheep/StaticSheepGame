--[[ Draw Library: GUI wrapper and helper functions ]]

local pairs = pairs
local table = table
local surface = surface
local string = string
local math = math
local setmetatable = setmetatable
local Color = Color
local tostring = tostring
local surface = surface

module( "draw" )

TEXT_ALIGN_LEFT = 0
TEXT_ALIGN_CENTER = 1
TEXT_ALIGN_RIGHT = 2
TEXT_ALIGN_TOP = 3
TEXT_ALIGN_BOTTOM = 4

local Corner8   = nil 
local Corner16  = nil 

function UpdateCorners()
  Corner8 = surface.GetTextureID("gui/corner8.png")
  Corner16 = surface.GetTextureID("gui/corner16.png")
end

function SimpleText(text, font, x, y, size, color, xalign, yalign)

  text = tostring( text )
  font = font or "Arial"
  x = x or 0
  y = y or 0
  xalign = xalign or TEXT_ALIGN_LEFT
  yalign = yalign or TEXT_ALIGN_TOP


  -- local w, h = surface.GetTextSize( text )

  -- if (xalign == TEXT_ALIGN_CENTER) then
  --   x = x - w/2
  -- elseif (xalign == TEXT_ALIGN_RIGHT) then
  --   x = x - w
  -- end
  
  -- if (yalign == TEXT_ALIGN_CENTER) then
  --   y = y - h/2
  -- elseif ( yalign == TEXT_ALIGN_BOTTOM ) then
  --   y = y - h
  -- end
  
  surface.SetPos( math.ceil( x ), math.ceil( y ) );
  
  if (color ~= nil) then
    local alpha = 255
    if (color.a) then alpha = color.a end

    surface.SetColor( color.r, color.g, color.b, alpha )
  else
    surface.SetColor(255, 255, 255, 255)
  end
  
  surface.DrawString(text, size, font)
  
  return w, h
  
end

local gmatch = string.gmatch
local find = string.find
local ceil = math.ceil
local GetTextSize = surface.GetTextSize
local max = math.max
function DrawText(text, font, x, y, size, color, xalign )

  if (font == nil) then font = "Arial" end
  if (text != nil) then text = tostring( text ) end
  if (x == nil) then x = 0 end
  if (y == nil) then y = 0 end

  local curX = x
  local curY = y
  local curString = ""
  
  --surface.SetFont(font)
  local sizeX, lineHeight = 32 --GetTextSize("\n")
  local tabWidth = 50
  
  for str in gmatch( text, "[^\n]*" ) do
    if #str > 0 then
      if find( str, "\t" ) then -- there's tabs, some more calculations required
        for tabs, str2 in gmatch( str, "(\t*)([^\t]*)" ) do
          curX = ceil( (curX + tabWidth * max(#tabs-1,0)) / tabWidth ) * tabWidth
          
          if #str2 > 0 then
            SimpleText( str2, font, curX, curY, color, xalign )
          
            local w, _ = GetTextSize( str2 )
            curX = curX + w
          end
        end
      else -- there's no tabs, this is easy
        SimpleText( str, font, curX, curY, size, color, xalign )
      end
      
      curX = x
      curY = curY + (lineHeight/2)
    end
  end
end

function RoundedBox( bordersize, x, y, w, h, color )

  return RoundedBoxEx( bordersize, x, y, w, h, color, true, true, true, true )

end

function RoundedBoxEx( bordersize, x, y, w, h, color, a, b, c, d )

  x = math.Round( x )
  y = math.Round( y )
  w = math.Round( w )
  h = math.Round( h )

  surface.SetColor( color.r, color.g, color.b, color.a )
  
  -- Draw as much of the rect as we can without textures
  surface.DrawRect( x+bordersize, y, w-bordersize*2, h )
  surface.DrawRect( x, y+bordersize, bordersize, h-bordersize*2 )
  surface.DrawRect( x+w-bordersize, y+bordersize, bordersize, h-bordersize*2 )
  
  if Corner8 == nil then
    UpdateCorners()
  end

  local tex = Corner8
  if ( bordersize > 8 ) then tex = Corner16 end
  
  surface.SetTexture( tex )
  
  if ( a ) then
    surface.DrawTexturedRectRotated( x + bordersize/2 , y + bordersize/2, bordersize, bordersize, 0 ) 
  else
    surface.DrawRect( x, y, bordersize, bordersize )
  end
  
  if ( b ) then
    surface.DrawTexturedRectRotated( x + w - bordersize/2 , y + bordersize/2, bordersize, bordersize, math.pi * 1.5 ) 
  else
    surface.DrawRect( x + w - bordersize, y, bordersize, bordersize )
  end
 
  if ( c ) then
    surface.DrawTexturedRectRotated( x + bordersize/2 , y + h -bordersize/2, bordersize, bordersize, math.pi * 0.5 )
  else
    surface.DrawRect( x, y + h - bordersize, bordersize, bordersize )
  end
 
  if ( d ) then
    surface.DrawTexturedRectRotated( x + w - bordersize/2 , y + h - bordersize/2, bordersize, bordersize, math.pi )
  else
    surface.DrawRect( x + w - bordersize, y + h - bordersize, bordersize, bordersize )
  end
  
end