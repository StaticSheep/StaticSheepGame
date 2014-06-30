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
    }
  }
}