local META = GetMeta("Vector2")

function META.__add(a, b)
  if type(a) == "number" then
    return META.new(b.x + a, b.y + a)
  elseif type(b) == "number" then
    return META.new(a.x + b, a.y + b)
  else
    return Vector.new(a.x + b.x, a.y + b.y)
  end
end

function META.__sub(a, b)
  if type(a) == "number" then
    return META.new(b.x - a, b.y - a)
  elseif type(b) == "number" then
    return META.new(a.x - b, a.y - b)
  else
    return Vector.new(a.x - b.x, a.y - b.y)
  end
end

function META.__mul(a, b)
  if type(a) == "number" then
    return META.new(b.x * a, b.y * a)
  elseif type(b) == "number" then
    return META.new(a.x * b, a.y * b)
  else
    return Vector.new(a.x * b.x, a.y * b.y)
  end
end

function META.__div(a, b)
  if type(a) == "number" then
    return META.new(b.x / a, b.y / a)
  elseif type(b) == "number" then
    return META.new(a.x / b, a.y / b)
  else
    return Vector.new(a.x / b.x, a.y / b.y)
  end
end

function META.__eq(a, b)
  return a.x == b.x and a.y == b.y
end

function META.__tostring(a)
  return "(".. a.x .. ", " .. a.y ..")"
end

function META.new(x, y)
  local tab = setmetatable({}, META)
  tab.x = x
  tab.y = y
  return tab
end

function META.distance(a, b)
  return (b - a):len()
end

function META:clone()
  return META.new(self.x, self.y)
end

function META:unpack()
  return self.x, self.y
end

function META:len()
  return math.sqrt(self.x * self.x + self.y * self.y)
end

function META:lenSq()
  return self.x * self.x + self.y * self.y
end

function META:Normalize()
  local len = self:len()
  self.x = self.x / len
  self.y = self.y / len
  return self
end

function META:normalized()
  return self / self:len()
end

function META:rotate(theta)
  local c = math.cos(theta)
  local s = math.sin(theta)

  self.x = c * self.x - s * self.y
  self.y = s * self.x + c * self.y
  return self
end

function META:rotated(theta)
  return self:clone():rotate(theta)
end

function META:perpendicular()
  return META.new(-self.y, self.x)
end

function META:projectOn(b)
  return (self * b) * b / b:lenSq()
end

function META:cross(b)
  return self.x * b.y - self.y * b.x
end

function META:__call(...)
  return META.new(...)
end

function META:__set(x, y)
  self.x = x
  self.y = y
end

Vector2 = setmetatable(META, META)
