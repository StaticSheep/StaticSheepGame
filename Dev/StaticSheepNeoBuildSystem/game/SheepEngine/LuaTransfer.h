/*****************************************************************
Filename: LuaTransfer.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  namespace Lua
  {
    void IntToLua(lua_State* L, Variable& var);
    void UIntToLua(lua_State* L, Variable& var);
    void FloatToLua(lua_State* L, Variable& var);
    void CharPToLua(lua_State* L, Variable& var);
    void StringToLua(lua_State* L, Variable& var);

    // All these really do is convert pointer -> solid data
    void GameComponentToLua(lua_State* L, Variable& var);
    void GameObjectToLua(lua_State* L, Variable& var);
    void GameSpaceToLua(lua_State* L, Variable& var);

    void IntFromLua(lua_State* L, int index, Variable* ref);
    void FloatFromLua(lua_State* L, int index, Variable* ref);
    void CharPFromLua(lua_State* L, int index, Variable* ref);
    void StringFromLua(lua_State* L, int index, Variable* ref);

    void GenericObjectFromLua(lua_State* L, int index, Variable* ref);

    
  }
}