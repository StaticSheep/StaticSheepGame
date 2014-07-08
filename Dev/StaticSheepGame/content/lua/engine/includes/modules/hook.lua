--[[--------------------------------------------------------------------
                  Static Sheep Lua Interface     
                  Hook system                      
----------------------------------------------------------------------]]
local pairs = pairs
local type = type
local IsValid = IsValid

module("hook")

local Hooks = {}

function GetTable() return Hooks end

function Add(event, name, func)
	if type(func) ~= "function" then return end
	if type(event) ~= "string" then return end

	if Hooks[event] == nil then
		Hooks[event] = {}
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
	local HookTable = Hooks[event]

	if HookTable ~= nil then

		for k,v in pairs(HookTable) do

			if type(k) == "string" then

				v(...)

			else

				if IsValid(k) then -- Assume it is a component or object

					-- If the object is valid, pass the first argument as self
					v(k, ...)

				else
					-- If the object is not valid, remove the hook

					HookTable[k] = nil
				end

			end

		end

	end
end

