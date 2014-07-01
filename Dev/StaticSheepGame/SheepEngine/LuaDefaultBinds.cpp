/*****************************************************************
Filename: LuaDefaultBinds.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  namespace Lua
  {
    void BindDefaultFunctions()
    {
      BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaError, LuaError);
      BIND_FUNCTION_EX(ENGINE->Lua(), LuaGetComponent, GetComponent);
      BIND_FUNCTION_EX(ENGINE->Lua(), RemoveObjectFromEngine, DestroyObject);
      BIND_FUNCTION_EX(ENGINE->Lua(), Transform::Test, Test);
    }
  }
}