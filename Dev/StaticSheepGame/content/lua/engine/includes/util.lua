-- local __pairs = pairs
-- function pairs(value)
--   if type(value) == "userdata" then
--     local g = getmetatable(value)
--     return g.__pairs(value)
--   else
--     return __pairs(value)
--   end
-- end

local function Pad()
  for i=1,padding do
    io.write("  ")
  end
end

function PrintTable(tbl)
  if not padding then padding = 0 end

  Pad()
  print("{")

  padding = padding + 1

  for k,v in pairs(tbl) do

    if (type(v) == "table") then
      Pad()
      io.write(tostring(k)..":".."\n")
      PrintTable(v)
    else
      Pad()
      io.write(tostring(k)..": "..tostring(v).. "\n")
    end

  end

  padding = padding - 1

  Pad()
  print("}")
end

function loopcheck(table)
  for k,v in pairs(table) do
  end
end

function IsValid(object)
  if object == nil then return false end
  if type(object) == "table" then
    if not pcall(loopcheck, object) then
      return false
    end
  end
  return type(object) == "userdata" or type(object) == "table"
end

if not module then
    function module(modname,...)
        local function findtable(tbl,fname)
            for key in string.gmatch(fname,"([%w_]+)") do
                if key and key~="" then
                    local val = rawget(tbl,key)
                    if not val then
                        local field = {}
                        tbl[key]=field
                        tbl = field
                    elseif type(val)~="table" then
                        return nil
                    else
                        tbl = val
                    end
                end
            end
            return tbl
        end

        assert(type(modname)=="string")
        local value,modul = package.loaded[modname]
        if type(value)~="table" then
            modul = findtable(_G,modname)
            assert(modul,"name conflict for module '"..modname.."'" )
            package.loaded[modname] = modul
            SheepModules[modname] = modul
        else
            modul = value
        end

        local name = modul._NAME
        if not name then
            modul._M = modul
            modul._NAME = modname
            modul._PACKAGE = string.match(modname,"([%w%._]*)%.[%w_]*$")
        end
        local func = debug.getinfo(2,"f").func
        debug.setupvalue(func,1,modul)
        for _,f in ipairs{...} do f(modul) end
    end

    function package.seeall(modul)
        setmetatable(modul,{__index=_G})
    end
end

function SetupMetatable(key, meta)

  function meta.__index(self, key, ...)
    _R.METAVALUES[tostring(self)] = _R.METAVALUES[tostring(self)] or {}

    if _R.METAVALUES[tostring(self)][key] then
      --print("Found meta value: ".. key)
      return _R.METAVALUES[tostring(self)][key]
    end

    --print("Didn't find meta value: ".. key)

    return meta[key]
  end

  function meta.__newindex(self, key, value)
    _R.METAVALUES[tostring(self)] = _R.METAVALUES[tostring(self)] or {}

    --print("Insert new index into meta table, key: "..key.." value: "..tostring(value) )

    _R.METAVALUES[tostring(self)][key] = value
  end

  function meta:__gc()
    _R.METAVALUES[tostring(self)] = nil
  end

  function meta:__next(k)
    return next(_R.METAVALUES[tostring(self)], k)
  end

  function meta:__pairs()
    return meta.__next, self, nil
  end

  function meta:SetupHooks()
  end

  function meta:Init()
    self:SetupHooks()
  end

  function meta.__New()
    return setmetatable({}, meta)
  end

end


function SetupMetatables()
  _R.METAVALUES = {}

  for key, meta in pairs(_R) do
    SetupMetatable(key, meta)
  end
end

function GetMeta(name)
  if _R["__"..name.."_MT"] == nil then
    print("Metatable not found for: \""..name.."\" creating one!")
    _R["__"..name.."_MT"] = {}
    
    SetupMetatable(name, _R["__"..name.."_MT"])
  end
  
  return _R["__"..name.."_MT"]
end

function RunString(command, ...)
  load("return function(...) " .. command .. " end")()(...)
end