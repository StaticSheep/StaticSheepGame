/*****************************************************************
Filename: LuaDefaultBinds.h
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once



namespace Framework
{
  namespace Lua
  {
    void BindLibraryFunctions(lua_State*);
    void BindDefaultFunctions(lua_State*);
  }
}