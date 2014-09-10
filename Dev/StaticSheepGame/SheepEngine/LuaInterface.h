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
    void CallStaticFuncFinal(lua_State* L, const char* funcName, Variable* args, size_t argCount);
    
    void CallMemberFuncFinal(lua_State* L, Variable& member, const char* funcName, Variable* args, size_t argCount);

    // Function calling overloads
    void CallFunc(lua_State* L, const char* funcName);

    template<typename A1>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1);

    template<typename A1, typename A2>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2);

    template<typename A1, typename A2, typename A3>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2, A3 arg3);

    template<typename A1, typename A2, typename A3, typename A4>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2, A3 arg3, A4 arg4);

    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5);

    void CallMemberFunc(lua_State* L, Variable& var, const char* funcName);

    void GenericToLua(lua_State* L, Variable& var);
    void GenericFromLua(lua_State* L, int index, Variable* var);

    void BindFunctionToLua(lua_State* L, Function* fn, const char* name, const char* table = nullptr);

    int SetPath(lua_State* L, const char* path);

    void StackDump(lua_State* L);

    void CreateNewGTable(lua_State* L, const char* name);
    void GenerateComponentTable(lua_State* L);

    template<typename A1>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1)
    {
      Variable args[] = {arg1};
      
      CallStaticFuncFinal(L, funcName, args, 1);
    }

    template<typename A1, typename A2>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2)
    {
      Variable args[] = {arg1, arg2};

      CallStaticFuncFinal(L, funcName, args, 2);
    }

    template<typename A1, typename A2, typename A3>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2, A3 arg3)
    {
      Variable args[] = {arg1, arg2, arg3};

      CallStaticFuncFinal(L, funcName, args, 3);
    }

    template<typename A1, typename A2, typename A3, typename A4>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2, A3 arg3, A4 arg4)
    {
      Variable args[] = {arg1, arg2, arg3, arg4};

      CallStaticFuncFinal(L, funcName, args, 4);
    }

    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    void CallFunc(lua_State* L, const char* funcName, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5)
    {
      Variable args[] = {arg1, arg2, arg3, arg4, arg5};

      CallStaticFuncFinal(L, funcName, args, 5);
    }

    void Shutdown(lua_State* L);

  }
}