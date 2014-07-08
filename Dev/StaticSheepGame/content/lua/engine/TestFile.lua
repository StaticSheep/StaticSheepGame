function TestHook(self)
  --TransformTest()
  --self:Test()
end

function Test()
  OBJTEST = GetComponent("Stand alone space", 3, "Transform")
  OBJTEST.blah = 10
  OBJTEST:TransformTest()
  OBJTEST:DoSomeLuaStuff()
end

function Test2()
  print(OBJTEST)
  print(OBJTEST.test)
  OBJTEST:TransformTest()
end