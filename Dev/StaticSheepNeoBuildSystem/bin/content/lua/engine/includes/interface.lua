--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface                           
----------------------------------------------------------------------]]

print("=================== Static Sheep Lua ===================")
print("Loading engine lua interface...")

if (not CONTENT_PATH) then CONTENT_PATH = package.path end

if (not LUA_PATH) then LUA_PATH = CONTENT_PATH .. "/lua/" end

if (not PATHS) then PATHS = {} end

if (not _R) then _R = {} end

function AddPath(path)
  PATHS[#PATHS + 1] = CONTENT_PATH .. path
  package.path = package.path .. ";" .. CONTENT_PATH .. path .. "?.lua"
end

function include(file)
  local filePath, attr

  for k,v in pairs(PATHS) do
    filePath = v .. file;
    attr = lfs.attributes( filePath )
    if attr ~= nil then
      dofile(filePath)
      return
    end
  end

  if (filesystem ~= nil) then
    if filesystem.LoadLuaFile(LUA_PATH, file) then return end
  end

  dofile(LUA_PATH .. file)
end

AddPath("/lua/engine/includes/modules/")
AddPath("/lua/engine/includes/libraries/")
AddPath("/lua/engine/includes/management/")
AddPath("/lua/engine/includes/")
AddPath("/lua/")

-- Initialize some tables
SheepModules = {}
GameSpaces = {}
LuaSpaces = {} -- Pure lua objects
LuaComponents = {}

include("util.lua")
include("meta.lua")

require("hook")
require("filesystem")

-- Initialize all of the modules
for k,v in pairs(SheepModules) do
  if v.Init then
    v.Init()
  end
end

filesystem.BlacklistFolder("includes")
filesystem.BlacklistFolder("lua_levels")

filesystem.LoadLuaFiles("content/lua/engine/includes/libraries/")
filesystem.LoadLuaFiles("content/lua/engine/includes/management/")



local function FileReloaded()
  ReloadObjects()
  ReloadComponents()
  gui.Reloaded()
end

function CheckOldFiles()
  filesystem.UpdateOldFiles(FileReloaded)
end

function LuaLoaded()
end

function PostInit()
  QueryComponents()
end


hook.Add("LogicUpdate", "CheckFiles", CheckOldFiles)


