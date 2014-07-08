local META = GetMeta("Transform")


function META:DoSomeLuaStuff()
  self:TransformTest()
  print("lua meta function")
end