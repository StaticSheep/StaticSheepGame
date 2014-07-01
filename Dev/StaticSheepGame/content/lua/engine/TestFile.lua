function TestHook(self)
	self:Test()
end

function Test()
	print("Test function ran!")
	local o = GetComponent("Stand alone space", 3, "Transform")
	hook.Add("test", o, TestHook)
	hook.Call("test")
end