/*****************************************************************
Filename: EngineLua.cpp
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "pch/precompiled.h"

#include "engine/core/Engine.h"
#include "types/space/Space.h"
#include <time.h>

namespace Framework
{

  lua_State* Engine::Lua() const
  {
    return L;
  }

  void Engine::LoadLuaLevel(const char* path)
  {
    m_loadLuaLevels = true;
    m_luaLevelsToLoad.push(path);
  }

  void Engine::LoadLuaLevels()
  {
    std::string luaLevel;

    while (!m_luaLevelsToLoad.empty())
    {
      luaLevel = m_luaLevelsToLoad.top();
      Lua::CallFunc(L, "LoadLuaLevel", luaLevel.c_str());
      m_luaLevelsToLoad.pop();
    }

    m_loadLuaLevels = false;
  }

  GameSpace* Engine::LuaCreateSpace(const char* name)
  {
    GameSpace* space = ENGINE->GetSpace(name);
    if (space == nullptr)
    {
      // Creates the space
      return ENGINE->CreateSpace(name);
    }
    else
    {
      std::string realName = name;
      realName += "[nametaken]";
      // Creates the space
      return ENGINE->LuaCreateSpace(realName.c_str());
    }
    //return space;
  }

  GameSpace* Engine::LuaGetSpace(const char* name)
  {
    return ENGINE->GetSpace(name);
  }

  GameComponent* Engine::LuaGetComponent(const char* name, unsigned int handle, const char* type)
  {
    GameSpace* space = (GameSpace*)ENGINE->GetSpace(name);

    ErrorIf(space == nullptr, "Engine-Lua Interface", "Space not found: %s", name);

    EComponent cType = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
    return space->GetHandles().GetAs<GameObject>(handle)->GetComponent(cType);
  }

  void Engine::LuaRemoveObjectFromEngine(const char* space, unsigned int handle)
  {
    ENGINE->GetSpace(space)->GetHandles().GetAs<GameObject>(handle)->Destroy();
  }

  void Engine::LuaQuit()
  {
    ENGINE->Quit();
  }

  void Engine::LuaSetVariable(Generic* obj, Member* member, int dummy)
  {
    Variable value;

    void* buffer = alloca(member->Type()->Size());

    new (&value) Variable(member->Type(), buffer);
    value.PlacementNew();

    value.FromLua(ENGINE->Lua(), 3); // 2 + 1 (Argument 2 [zero based], + 1) 

    member->Type()->PlacementCopy((char*)obj + member->Offset(), buffer);
  }

  void Engine::LuaGetVariable(Generic* obj, Member* member)
  {
    Variable value(member->Type(), (char*)obj + member->Offset());

    value.ToLua(ENGINE->Lua());
  }

  void Engine::LuaClearSpaces()
  {
    ENGINE->ClearSpaces();
  }

  void Engine::LuaRemoveSpace(const char* name)
  {
    ENGINE->GetSpace(name)->m_valid = false;
  }

  void Engine::LuaGetComponentList(int count, int dummy)
  {
    lua_State* L = ENGINE->Lua();
    ENGINE->m_luaComponentList.clear();
    //Lua::StackDump(L);

    std::string entry;
    for (int i = 1; i <= count; ++i)
    {
      lua_pushinteger(L, i);
      lua_gettable(L, -2);
      //Lua::StackDump(L);
      entry = lua_tostring(L, -1);
      ENGINE->m_luaComponentList.push_back(entry);
      //Lua::StackDump(L);
      lua_pop(L, 1);
    }

    lua_settop(L, 0);
    //Lua::StackDump(L);
  }

  void Engine::LuaError(const char* msg)
  {
    TRACELOG->Log(TraceLevel::ERR, "LuaError: %s", msg);
    //FORCEERROR("LuaError", "%s", msg);
  }

  const char* Engine::DateTime()
  {
    // get the current time in seconds since 1970
    time_t currentTime;
    time(&currentTime);

    struct tm* dateTime;

    // convert it into local time
    dateTime = localtime(&currentTime);

    // convert that into ascii
    char* theTime = asctime(dateTime);

    // then get some char* to alter the output
    char* reader = theTime;
    char* begin;
    char* last;

    // keeps track of the amount of spaces
    int count = 0;

    // iterate through theTime 
    while (reader++)
    {
      // we are looking for the \n because we want to get rid of it
      if (*reader == '\n')
      {
        *reader = '\0';
        break;
      }

      // also if we find a space...
      if (*reader == ' ')
      {
        ++count;

        // keep track of the last space
        last = reader;

        // to get rid of the month and day
        if (count == 3)
          begin = reader + 1;
      }
    }
    // to get rid of the year
    *last = '\0';

    return begin;
  }
}