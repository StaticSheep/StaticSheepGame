/*****************************************************************
Filename: Vec2.cpp
Project: 
Author(s):

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "precompiled.h"
namespace Framework
{
  void Vec2::ToLua(lua_State* L, Variable& var)
  {
    // Establish a reference to a new set of userdata
    //Variable* ref = (Variable*)lua_newuserdata(L, sizeof(Variable));
    //new (ref) Variable(var); // copies stuff

    //Lua::StackDump(L);

    lua_pushcfunction(L, Lua::ErrorFunc); // Index 1

    //Lua::StackDump(L);

    // Create a new table
    lua_createtable(L, 2, 0); // Index 2

    // Get the meta table
    lua_getglobal(L, "_R"); // index 3
    lua_getfield(L, -1, "__Vector2_MT"); // index 4

    lua_getfield(L, -1, "new"); // Index 5

    // Push arguments
    lua_pushnumber(L, var.GetValue<Vec2>().X); // Index 6
    lua_pushnumber(L, var.GetValue<Vec2>().Y); // Index 7

    //Lua::StackDump(L);

    lua_pcall(L, 2, 1, -7);

    //Lua::StackDump(L);

  }

  void Vec2::FromLua(lua_State* L, int index, Variable* var)
  {
    //Lua::StackDump(L);

    lua_getfield(L, -1, "x");
    var->GetValue<Vec2>().X = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "y");
    var->GetValue<Vec2>().Y = (float)lua_tonumber(L, -1);
    lua_pop(L, 1);
  }
}