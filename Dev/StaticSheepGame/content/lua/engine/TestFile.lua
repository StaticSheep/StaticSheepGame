function TestHook(self)
	--TransformTest()
	--self:Test()
end

function Test()
	print("Test function ran!")
	local o = GetComponent("Stand alone space", 3, "Transform")
	PrintTable(o)
	hook.Add("test", o, TestHook)
	hook.Call("test")
	testlib.DoSomething()
end