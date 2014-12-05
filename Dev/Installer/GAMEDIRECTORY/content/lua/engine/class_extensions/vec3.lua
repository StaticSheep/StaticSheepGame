local META = GetMeta("Vector3")

function META.__add(a, b)
  --print("Vec3 Add")
  if type(a) == "number" then
    return META.new(b.x + a, b.y + a, b.z + a)
  elseif type(b) == "number" then
    return META.new(a.x + b, a.y + b, a.z + b)
  else
    -- print("Vec3 Add 2 Vec3's")
    -- for k,v in pairs(a) do
    --   print(k.."-"..v)
    -- end
    
    --PrintTable(getmetatable(a))

    -- function trace (event, line)
    --   local s = debug.getinfo(2).short_src
    --   print(s .. ":" .. line)
    -- end
    
    -- debug.sethook(trace, "l")

    -- print(tostring(a))
    -- local status, err = pcall(function() print(a.z) end)
    
    -- print(debug.traceback())

    -- debug.sethook(nil, "l")
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z)
  end
end

function META.__sub(a, b)
  if type(b) == "number" then
    return META.new(a.x - b, a.y - b, a.z - b)
  else
    return META.new(a.x - b.x, a.y - b.y, a.z - b.z)
  end
end

function META.__mul(a, b)
  if type(a) == "number" then
    return META.new(b.x * a, b.y * a, b.z * a)
  elseif type(b) == "number" then
    return META.new(a.x * b, a.y * b, a.z * b)
  else
    return META.new(a.x * b.x, a.y * b.y, a.z * b.z)
  end
end

function META.__div(a, b)
  if type(b) == "number" then
    return META.new(a.x / b, a.y / b, a.z / b)
  else
    return META.new(a.x / b.x, a.y / b.y, a.z / b.z)
  end
end

function META.__eq(a, b)
  return a.x == b.x and a.y == b.y and a.z == b.z
end


function META.new(x, y, z)
  --print("New Vec3")
  local tab = setmetatable({}, META)
  tab.x = x
  tab.y = y
  tab.z = z
  return tab
end

function META.distance(a, b)
  return (b - a):len()
end

function META:clone()
  return META.new(self.x, self.y, self.z)
end

function META:unpack()
  return self.x, self.y, self.z
end

function META:len()
  return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)
end

function META:lenSq()
  return self.x * self.x + self.y * self.y + self.z * self.z
end

function META:Normalize()
  local len = self:len()
  self.x = self.x / len
  self.y = self.y / len
  self.z = self.z / len
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
  return META.new(-self.y, self.x, self.z)
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
  self.z = z
end

Vector3 = setmetatable(META, META)
Vec3 = setmetatable(META, META)
