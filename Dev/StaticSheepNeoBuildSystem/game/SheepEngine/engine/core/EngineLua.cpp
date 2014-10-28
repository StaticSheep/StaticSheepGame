#include "pch/precompiled.h"

#include "engine/core/Engine.h"
#include "types/space/Space.h"

namespace Framework
{

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
}