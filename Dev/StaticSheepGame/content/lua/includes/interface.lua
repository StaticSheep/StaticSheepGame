--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface                           
----------------------------------------------------------------------]]

if (not CONTENT_PATH) then CONTENT_PATH = package.path end


function AddPath(path)
  package.path = package.path .. ";" .. CONTENT_PATH .. path
end

function include(file)
  dofile(CONTENT_PATH .. "/lua/" .. file)
end

AddPath("/lua/includes/modules/?.lua")
AddPath("/lua/includes/?.lua")
AddPath("/lua/?.lua")

-- Initialize some tables
SheepModules = {}
Spaces = {}

include("includes/util.lua")

require("filesystem")
require("hook")


-- Initialize all of the modules
for k,v in pairs(SheepModules) do
  if v.Init then
    v.Init()
  end
end

hook.Add("LogicUpdate", "CheckFiles", filesystem.UpdateOldFiles)


