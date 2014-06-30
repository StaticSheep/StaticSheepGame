/*****************************************************************
Filename: LuaTransfer.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

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

    void ComponentToLua(lua_State* L, Variable& var)
    {
      GameComponent* comp = (GameComponent*)var.GetData();
      comp->typeinfo->ToLua(ENGINE->Lua(), Variable(comp->typeinfo, comp));
    }




    void IntFromLua(lua_State* L, int index, Variable* ref)
    {
      ErrorIf(!lua_isnumber(L, index), "LuaTransfer", "Type mismatch!");

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
      *var = temp;
    }

    void StringFromLua(lua_State* L, int index, Variable* var)
    {
      ErrorIf(!lua_isstring(L, index), "LuaTransfer", "Type mismatch! Wrong argument type lua->C++ ?");
      std::string temp(lua_tostring(L, index));
      var->GetTypeInfo()->PlacementCopy(var->GetData(), &temp);
    }



  }
}