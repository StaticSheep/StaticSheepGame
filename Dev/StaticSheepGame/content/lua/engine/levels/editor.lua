-- Static Sheep Editor --
print("\n=== Loading Editor Lua Level ===")

local space = CreateLuaSpace("EditorSpace")
local obj = space:CreateObject("BasicObject")
local gspace = engine.CreateSpace("test")
local object = factory.CreateObjectFromType(gspace, "test_type")
gspace:RemoveObject(object)

object = gspace:CreateObject("test_type")
gspace:RemoveObject(object)
