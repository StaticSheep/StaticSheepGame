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
      
      //BIND_FUNCTION_EX(ENGINE->Lua(), Transform::Test, TransformTest);
      //BIND_FUNCTION_EX(ENGINE->Lua(), Engine::DoSomething, engineDoSomething);

      BIND_FUNCTION_EX(ENGINE->Lua(), LuaComponent::ReceiveLoadCommend, SendLoadCommand);

      BIND_FUNCTION_EX(ENGINE->Lua(), GameObject::Destroy, Destroy);
      BIND_FUNCTION_EX(ENGINE->Lua(), GameObject::LuaGetComponent, GetComponent);


      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::CopyGameSpace, MakeCopy);
      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::CreateObjectFromArchetype, CreateObject);
      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::GetGameObject, GetObject);
      BIND_FUNCTION_EX(ENGINE->Lua(), GameSpace::Clear, Clear);



      CREATE_TABLE(ENGINE->Lua(), engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaError, Error, engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaQuit, Quit, engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaGetSpace, GetSpace, engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaCreateSpace, CreateSpace, engine);
      //BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaRemoveSpace, RemoveSpace, engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaGetComponent, GetComponent, engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaRemoveObjectFromEngine, DestroyObject, engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaSetVariable, SetVariable, engine);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Engine::LuaGetVariable, GetVariable, engine);

      CREATE_TABLE(ENGINE->Lua(), surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::SetColor, SetColorEx, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::SetRotation, SetRotation, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::SetTexture, SetTexture, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::SetUseCamera, SetUseCamera, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::GetTextureID, GetTextureID, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::DrawRect, DrawRect, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::DrawTexturedRect, DrawTexturedRect, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::DrawLine, DrawLine, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::DrawString, DrawString, surface);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Draw::SetPosition, SetPos, surface);

      CREATE_TABLE(ENGINE->Lua(), factory);
      BIND_FUNCTION_TABLE(ENGINE->Lua(), Factory::LuaLoadObjectFromArchetype, CreateObjectFromType, factory);

    }
  }
}