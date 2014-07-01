/*****************************************************************
Filename: LuaInterface.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  namespace Lua
  {

    int GenericFunction(lua_State* L);

    // Loads a lua file via path name
    void LoadFile(lua_State* L, const char* name);

    // Creates a new lua environment
    lua_State* CreateEnvironment(void);

    // Function called when a lua error occurs
    int ErrorFunc(lua_State* L);

    // Final step before calling a lua function
    void CallFuncFinal(lua_State* L, const char* funcName, Variable* args, size_t argCount);
    // Function calling overloads
    void CallFunc(lua_State* L, const char* funcName);
    template<typename A1>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1);

    void GenericToLua(lua_State* L, Variable& var);
    void GenericFromLua(lua_State* L, int index, Variable* var);

    void BindFunctionToLua(lua_State* L, Function* fn, const char* name);

    int SetPath(lua_State* L, const char* path);

    void StackDump(lua_State* L);

    template<typename A1>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1)
    {
      Variable args[] = {arg1};
      
      CallFuncFinal(L, funcName, args, 1);
    }

    void Shutdown(lua_State* L);

  }
}