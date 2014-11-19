/*****************************************************************
Filename: LuaTransfer.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

namespace Framework
{
  namespace Lua
  {
    void IntToLua(lua_State* L, Variable& var)
    {
      lua_pushinteger(L, var.GetValue<int>());
    }

    void UIntToLua(lua_State* L, Variable& var)
    {
      lua_pushinteger(L, var.GetValue<unsigned int>());
    }

    void FloatToLua(lua_State* L, Variable& var)
    {
      lua_pushnumber(L, var.GetValue<float>());
    }

    void CharPToLua(lua_State* L, Variable& var)
    {
      lua_pushstring(L, var.GetValue<char*>());
    }

    void StringToLua(lua_State* L, Variable& var)
    {
      lua_pushstring(L, var.GetValue<std::string>().c_str());
    }

    void BoolToLua(lua_State* L, Variable& var)
    {
      lua_pushboolean(L, var.GetValue<bool>());
    }

    void GameComponentToLua(lua_State* L, Variable& var)
    {
      GameComponent* comp = (GameComponent*)var.GetData();
      comp->typeinfo->ToLua(ENGINE->Lua(), Variable(comp->typeinfo, comp));
    }

    void GameSpaceToLua(lua_State* L, Variable& var)
    {
      GameSpace* space = (GameSpace*)var.GetData();
      const TypeInfo* type = GET_TYPE(GameSpace);
      type->ToLua(ENGINE->Lua(), Variable(type, space));
    }

    void GameObjectToLua(lua_State* L, Variable& var)
    {
      GameObject* obj = (GameObject*)var.GetData();
      const TypeInfo* type = GET_TYPE(GameObject);
      type->ToLua(ENGINE->Lua(), Variable(type, obj));
    }

    void GenericObjectFromLua(lua_State* L, int index, Variable* ref)
    {
      Variable* var = ((Variable*)lua_touserdata(L, index));
      Generic* obj = var->GetValue<Generic*>();

      new (ref) Variable(var->GetTypeInfo(), obj->space->GetHandles().Get(obj->self));
    }




    void IntFromLua(lua_State* L, int index, Variable* ref)
    {
      //ErrorIf(!lua_isnumber(L, index), "LuaTransfer", "Type mismatch!");

      int temp = lua_tointeger(L, index);
      ref->GetTypeInfo()->PlacementCopy(ref->GetData(), &temp);
    }

    void FloatFromLua(lua_State* L, int index, Variable* var)
    {
      ErrorIf(!lua_isnumber(L, index), "LuaTransfer", "Type mismatch!");

      float temp = (float)lua_tonumber(L, index);
      var->GetTypeInfo()->PlacementCopy(var->GetData(), &temp);
    }

    void CharPFromLua(lua_State* L, int index, Variable* var)
    {
      ErrorIf(!lua_isstring(L, index), "LuaTransfer", "Type mismatch! Wrong argument type lua->C++ ?");
      const char* temp = lua_tostring(L, index);
      //var->SetData(temp);
      *var = temp;
    }

    void StringFromLua(lua_State* L, int index, Variable* var)
    {
      ErrorIf(!lua_isstring(L, index), "LuaTransfer", "Type mismatch! Wrong argument type lua->C++ ?");
      std::string temp(lua_tostring(L, index));
      var->GetTypeInfo()->Copy(var->GetData(), &temp);
    }

    void BoolFromLua(lua_State* L, int index, Variable* var)
    {
      int b = lua_toboolean(L, index);
      var->GetTypeInfo()->Copy(var->GetData(), &b);
    }


  }
}