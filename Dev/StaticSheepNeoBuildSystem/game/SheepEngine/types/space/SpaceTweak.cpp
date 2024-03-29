/*****************************************************************
Filename: SpaceTweak.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include <windows.h>
#include <commdlg.h>
#include <iostream>

#include "types/space/Space.h"
#include "types/object/Object.h"


#include "systems/anttweak/AntTweakModule.h"

namespace Framework
{

  // AntTweak bar creator
  void GameSpace::CustomTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
  {
#if USE_ANTTWEAKBAR
    // Figure out what to call the bar for tweaking game space
    GameSpace& space = var.GetValue<GameSpace>();
    std::string spaceName = "GameSpace";

    // Creates a bar for the space
    AntTweak::TBar* spaceBar = ATWEAK->CreateBar(space.GetName().c_str());
    space.tweakHandle = spaceBar->self;

    spaceBar->SetPos(225, 16);

    space.UpdateTweakBar();
#endif
  }

  static void TweakObjectCB(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);

    obj->TweakObject();
  }

  static void CreateObjectCB(void* clientData)
  {
    GameSpace* space = (GameSpace*)clientData;

    // Create an empty object with a transform component
    GameObject* obj = space->CreateEmptyObject();
    obj->AddComponent(space->CreateComponent(eTransform));
    obj->Initialize();

    space->UpdateTweakBar();
  }

  static void SaveSpaceCB(void* clientData)
  {
    GameSpace* space = (GameSpace*)clientData;
    
    FACTORY->SaveSpaceToFile(space, space->GetName().c_str(), true);

    std::cout << "Saved Space " << space->GetName().c_str() << " to file.\n";
  }

  static void DeleteSpaceCB(void* clientData)
  {
    GameSpace* space = (GameSpace*)clientData;
    space->Destroy();
  }

  static void LoadArchetypeFile(void* clientData)
  {
    GameSpace* space = (GameSpace*)clientData;

    OPENFILENAME ofn;
    char szFile[512];

    TCHAR Buffer[512];
    DWORD dwRet;

    dwRet = GetCurrentDirectory(512, Buffer);

    ZeroMemory( &ofn , sizeof( ofn));
    ofn.lStructSize = sizeof ( ofn );
    ofn.hwndOwner = NULL  ;
    ofn.lpstrFile = szFile ;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = "Archetype\0*.arche\0";
    ofn.nFilterIndex =1;
    ofn.lpstrFileTitle = NULL ;
    ofn.nMaxFileTitle = 0 ;
    ofn.lpstrInitialDir="content\\data\\archetypes";
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
    GetOpenFileName( &ofn );

    SetCurrentDirectory(Buffer);

    if (szFile[0] == 0)
      return;

    

    GameObject* obj = FACTORY->LoadObjectFromArchetypeFP(space, szFile);

    space->UpdateTweakBar();

	ErrorIf(!obj, "SpaceTweaker", "Failed to load object from archetype! Filepath: %s", szFile);

    obj->TweakObject();
  }

  // Updates the anttweak bar
  void GameSpace::UpdateTweakBar()
  {
    if (tweakHandle == Handle::null)
      return;

    AntTweak::TBar* spaceBar = ATWEAK->GetBar(tweakHandle);
    spaceBar->Reset();

    spaceBar->SetLabel(m_name.c_str());

    spaceBar->AddVarRW("Name", AntTweak::TW_TYPE_STDSTRING, &m_name);

    spaceBar->DefineLabel("Save Space to File");
    spaceBar->DefineKeyShortcut("CTRL+s");
    spaceBar->AddButton("SaveSpace", SaveSpaceCB, this);

    spaceBar->DefineLabel("New Object");
    spaceBar->AddButton("NewObject", CreateObjectCB, this);

    spaceBar->DefineLabel("Load Archetype");
    spaceBar->AddButton("LoadArchetype", LoadArchetypeFile, this);

    spaceBar->DefineGroup("Delete");
    spaceBar->DefineLabel("Delete Space");
    spaceBar->AddButton("DeleteSpace", DeleteSpaceCB, this);
    spaceBar->SetGroupOpened("Delete", false);

    spaceBar->AddSeparator("ObjectSeperator");

    std::string objName;
    for(size_t i=0; i < m_objects.Size(); ++i)
    {
      GameObject* obj = (GameObject*)m_objects[i];

      // Create a temporary object name
      objName = "Object";
      objName += std::to_string(i);
      // If the object actually has a name we label it that
      if (obj->name.length() > 0)
        spaceBar->DefineLabel(obj->name.c_str());


      GenericLookup* gl = obj->tweakLookup;

      if (gl == nullptr)
      {
        // Create a Generic Lookup struct for the object and have the object point to it.
        // We pass this lookup struct when the ObjectTweak button is hit so we can find the
        // object and tweak it
        gl = DBG_NEW GenericLookup();
        gl->self = obj->self;
        gl->space = obj->space;
        obj->tweakLookup = gl;
      }

      spaceBar->AddButton(objName.c_str(), TweakObjectCB, gl);
    }

  }
}