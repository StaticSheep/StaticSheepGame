/*****************************************************************
Filename: LuaDefaultBinds.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "engine/core/Macros.h"

#include "components/sound/CSoundEmitter.h"
#include "components/transform/CTransform.h"
#include "components/rigidbody/CRigidBody.h"
#include "components/colliders/CBoxCollider.h"
#include "components/sprites/CSprite.h"
#include "components/lua/CLuaComponent.h"

#include "systems/input/devices/GamepadInput.h"
#include "systems/input/Input.h"
#include "components/controllers/player/CPlayerController.h"
#include "components/gameplay_scripts/Level_Scripts/CLevel1_Logic.h"
#include "components/sound/CSoundPlayer.h"
#include "Components/controllers/chip/CChipController.h"

namespace Framework
{
  namespace Lua
  {
    
    void BindLibraryFunctions(lua_State* L)
    {
      CREATE_TABLE(L, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaError, Error, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaQuit, Quit, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaGetSpace, GetSpace, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaCreateSpace, CreateSpace, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaRemoveSpace, RemoveSpace, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaClearSpaces, Wipe, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaGetComponent, GetComponent, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaRemoveObjectFromEngine, DestroyObject, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaSetVariable, SetVariable, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaGetVariable, GetVariable, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaGetComponentList, SendComponentList, engine);
      BIND_FUNCTION_TABLE(L, Engine::DateTime, DateTime, engine);
      BIND_FUNCTION_TABLE(L, Engine::LuaChangeLevel, ChangeLevel, engine);

      CREATE_TABLE(L, surface);
      BIND_FUNCTION_TABLE(L, Draw::SetColor, SetColorEx, surface);
      BIND_FUNCTION_TABLE(L, Draw::SetRotation, SetRotation, surface);
      BIND_FUNCTION_TABLE(L, Draw::SetTexture, SetTexture, surface);
      BIND_FUNCTION_TABLE(L, Draw::SetCamState, SetCamState, surface);
      BIND_FUNCTION_TABLE(L, Draw::GetTextureID, GetTextureID, surface);
      BIND_FUNCTION_TABLE(L, Draw::GetTextSize, GetTextSize, surface);
      BIND_FUNCTION_TABLE(L, Draw::DrawRect, DrawRect, surface);
      BIND_FUNCTION_TABLE(L, Draw::DrawTexturedRect, DrawTexturedRect, surface);
      BIND_FUNCTION_TABLE(L, Draw::DrawTexturedRectRotated, DrawTexturedRectRotated, surface);
      BIND_FUNCTION_TABLE(L, Draw::DrawLine, DrawLine, surface);
      BIND_FUNCTION_TABLE(L, Draw::LuaDrawString, DrawString, surface);
      BIND_FUNCTION_TABLE(L, Draw::SetPosition, SetPos, surface);
      BIND_FUNCTION_TABLE(L, Draw::SetPositionEx, SetPosEx, surface);
      BIND_FUNCTION_TABLE(L, Draw::ForceZ, ForceZ, surface);
      BIND_FUNCTION_TABLE(L, Draw::LuaMeasureString, MeasureString, surface);
      BIND_FUNCTION_TABLE(L, Draw::GetFontIndex, GetFontID, surface);

      CREATE_TABLE(L, audio);
      BIND_FUNCTION_TABLE(L, SheepAudio::LuaPauseAll, PauseAll, audio);

      BIND_FUNCTION_EX(L, InputManager::GetGamePad, GamePad);

      BIND_FUNCTION_EX(L, InputManager::KeyIsDown, KeyIsDown);
      BIND_FUNCTION_EX(L, InputManager::KeyIsPressed, KeyIsPressed);
      BIND_FUNCTION_EX(L, InputManager::KeyIsReleased, KeyIsReleased);

      BIND_FUNCTION_EX(L, Draw::ScreenWidth, GetScrW);
      BIND_FUNCTION_EX(L, Draw::ScreenHeight, GetScrH);

      BIND_FUNCTION_EX(L, Engine::CurrentTime, CurTime);

      CREATE_TABLE(L, factory);
      BIND_FUNCTION_TABLE(L, Factory::LuaLoadObjectFromArchetype, CreateObjectFromType, factory);

    }

    void BindDefaultFunctions(lua_State* L)
    {

      //BIND_FUNCTION_EX(ENGINE->Lua(), Engine::LuaError, LuaError);

      BIND_FUNCTION_EX(L, Transform::SetTranslation, SetTranslation);
      BIND_FUNCTION_EX(L, Transform::SetRotation, SetRotation);
      BIND_FUNCTION_EX(L, Transform::SetScale, SetScale);
      BIND_FUNCTION_EX(L, Transform::GetTranslation, GetTranslation);
      BIND_FUNCTION_EX(L, Transform::GetRotation, GetRotation);
      BIND_FUNCTION_EX(L, Transform::GetScale, GetScale);

      //BIND_FUNCTION_EX(ENGINE->Lua(), Engine::DoSomething, engineDoSomething);

      BIND_FUNCTION_EX(L, SoundPlayer::LuaPlay, PlayLoop);
      BIND_FUNCTION_EX(L, SoundPlayer::LuaStop, StopLoop);

      BIND_FUNCTION_EX(L, LuaComponent::ReceiveLoadCommend, SendLoadCommand);

      BIND_FUNCTION_EX(L, GameObject::Destroy, Destroy);
      BIND_FUNCTION_EX(L, GameObject::LuaHasComponent, HasComponent);
      BIND_FUNCTION_EX(L, GameObject::LuaGetComponent, GetComponent);


      BIND_FUNCTION_EX(L, GameSpace::CopyGameSpace, MakeCopy);
      BIND_FUNCTION_EX(L, GameSpace::CreateObjectFromArchetype, CreateObject);
      BIND_FUNCTION_EX(L, GameSpace::GetGameObject, GetObject);
      BIND_FUNCTION_EX(L, GameSpace::SetPaused, SetPaused);
      BIND_FUNCTION_EX(L, GameSpace::Clear, Clear);

      BIND_FUNCTION_EX(L, GamePadInput::ButtonDown, ButtonDown);
      BIND_FUNCTION_EX(L, GamePadInput::ButtonPressed, ButtonPressed);
      BIND_FUNCTION_EX(L, GamePadInput::LeftStick, LeftStick);
      BIND_FUNCTION_EX(L, GamePadInput::RightStick, RightStick);
      BIND_FUNCTION_EX(L, GamePadInput::Connected, Connected);
      BIND_FUNCTION_EX(L, GamePadInput::LeftTrigger, LeftTrigger);
      BIND_FUNCTION_EX(L, GamePadInput::RightTrigger, RightTrigger);
      BIND_FUNCTION_EX(L, GamePadInput::Rumble, Rumble);
      BIND_FUNCTION_EX(L, GamePadInput::InDeadzone, InDeadzone);

      BIND_FUNCTION_EX(L, SoundEmitter::Play, Play);

      BIND_FUNCTION_EX(L, PlayerController::CurrentHealth, CurrentHealth);
      BIND_FUNCTION_EX(L, Level1_Logic::GetPlayerHealth, GetPlayerHealth);
      BIND_FUNCTION_EX(L, Level1_Logic::GetPlayerLives, GetPlayerLives);
      BIND_FUNCTION_EX(L, Level1_Logic::GetPlayerCoins, GetPlayerCoins);
      BIND_FUNCTION_EX(L, Level1_Logic::GetWinner, GetWinner);
      BIND_FUNCTION_EX(L, Level1_Logic::EndMatch, EndMatch);

      BIND_FUNCTION_EX(L, ChipController::GetPlayerChips, GetPlayerChips);

      //BIND_FUNCTION_EX(L, BoxCollider::SetVelocity, SetVelocity);
    }


  }
}