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
      //BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaError, LuaError);
      
      BIND_FUNCTION_EX(ENGINE->Lua(), Transform::Test, TransformTest);
      //BIND_FUNCTION_EX(ENGINE->Lua(), Engine::DoSomething, engineDoSomething);
      BIND_FUNCTION_EX(ENGINE->Lua(), LuaComponent::ReceiveLoadCommend, SendLoadCommand);


      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::CreateObjectFromArchetype, CreateObject);
      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::RemoveGameObject, RemoveObject);
      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::RemoveGameObjectByHandle, RemoveObjectByHandle);
      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::GetGameObject, GetObject);

      CREATE_TABLE(ENGINE->Lua(), "engine");
      BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaError, Error, "engine");
      BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaGetSpace, GetSpace, "engine");
      BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaCreateSpace, CreateSpace, "engine");
      BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaGetComponent, GetComponent, "engine");
      BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaRemoveObjectFromEngine, DestroyObject, "engine");

      CREATE_TABLE(ENGINE->Lua(), "factory");
      BIND_FUNCTION_EX(ENGINE->Lua(), Factory::LuaLoadObjectFromArchetype, CreateObjectFromType, "factory");
    }
  }
}