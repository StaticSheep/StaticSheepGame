--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface     
                  Hook system                      
----------------------------------------------------------------------]]
local pairs = pairs
local type = type
local IsValid = IsValid
local setmetatable = setmetatable


--local print = print
--local PrintTable = PrintTable

module("hook")

local Hooks = {}


function GetTable() return Hooks end

function Add(event, name, func)
	if type(func) ~= "function" and type(func) ~= "string" then return end
	if type(event) ~= "string" then return end
	
	if Hooks[event] == nil then
		Hooks[event] = setmetatable({}, {__mode = 'k' })
	end

	Hooks[event][name] = func
end

function Remove(event, name)
	if type(func) ~= "function" then return end
	if type(event) ~= "string" then return end

	Hooks[event][name] = nil
end

function Run(event, ...)
	return Call(name, ...)
end

function Call(event, ...)

	if Hooks[event] ~= nil then

		--PrintTable(HookTable)

		for k,v in pairs(Hooks[event]) do

			if type(k) == "string" then

				v(...)

			else

				if IsValid(k) then -- Assume it is a component or object

					-- If the object is valid, pass the first argument as self
					if (type(v) == "string") then
						k[v](k, ...)
					else
						v(k, ...)
					end

				else
					-- If the object is not valid, remove the hook
					--print("Invalid! Removing!")
					Hooks[event][k] = nil
				end

			end

		end

	end
end

