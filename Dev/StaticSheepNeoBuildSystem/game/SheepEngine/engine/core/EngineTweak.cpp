/*****************************************************************
Filename: EngineTweak.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "systems/anttweak/AntTweakModule.h"

#include <windows.h>
#include <commdlg.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include "systems/editor/GizmoEditor.h"


namespace Framework
{

  static Handle EngineBar;
  static int spaceNum = 0;

  static void EditorCreateSpace(void* clientData)
  {
    std::string spaceName("NewSpace");
    spaceName += std::to_string(spaceNum++);
    GameSpace* space = ENGINE->CreateSpace(spaceName.c_str());
    space->SetPaused(true);
    space->Tweak();
  }

  static void EditorOpenSpace(void* clientData)
  {
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
    ofn.lpstrFilter = "Sheep Spaces\0*.space\0";
    ofn.nFilterIndex =1;
    ofn.lpstrFileTitle = NULL ;
    ofn.nMaxFileTitle = 0 ;
    ofn.lpstrInitialDir="content\\data\\spaces";
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
    GetOpenFileName( &ofn );

    SetCurrentDirectory(Buffer);

    if (szFile[0] == 0)
      return;

    GameSpace* sp = FACTORY->LoadSpaceFilePath(szFile);
    sp->SetPaused(true);

    if (sp != nullptr)
      sp->Tweak();
  }

  static void TweakSpaceCB(GameSpace* space)
  {
    space->Tweak();
  }

  static void EditorLoadLevel(void* clienData)
  {
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
    ofn.lpstrFilter = "Sheep Levels\0*.level\0";
    ofn.nFilterIndex =1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0 ;
    ofn.lpstrInitialDir="content\\data\\levels";
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
    GetOpenFileName( &ofn );

    if (szFile[0] == 0)
      return;

    SetCurrentDirectory(Buffer);

    FACTORY->LoadLevelFilePath(szFile, TweakSpaceCB);
  }

  static void EditorSaveLevel(void* clienData)
  {
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
    ofn.lpstrFilter = "Sheep Level\0*.level\0";
    ofn.nFilterIndex =1;
    ofn.lpstrFileTitle = NULL ;
    ofn.nMaxFileTitle = 0 ;
    ofn.lpstrInitialDir="content\\data\\levels";
    ofn.Flags = OFN_PATHMUSTEXIST;
    GetSaveFileName(&ofn);

    if (szFile[0] == 0)
      return;

    SetCurrentDirectory(Buffer);

    FACTORY->SaveLevel(szFile);
  }

  static void EditorPlayLevel(void* clientData)
  {
    ATWEAK->RemoveAllBars();

    ENGINE->PlayInEditor(true);
  }

  static void SwitchGizmoTranslation(void* cd)
  {
    GIZMO_EDITOR->SetGizmoMode(Translation);
    ENGINE->UpdateEditorWindow();
  }

  static void SwitchGizmoScale(void* cd)
  {
    GIZMO_EDITOR->SetGizmoMode(Scale);
    ENGINE->UpdateEditorWindow();
  }

  static void SwitchGizmoRotation(void* cd)
  {
    GIZMO_EDITOR->SetGizmoMode(Rotation);
    ENGINE->UpdateEditorWindow();
  }

  void Engine::UpdateEditorWindow()
  {
    AntTweak::TBar* mainBar = ATWEAK->GetBar(EngineBar);
    mainBar->Reset();

    mainBar->DefineLabel("Load Level");
    mainBar->AddButton("LoadLevel", EditorLoadLevel, nullptr);

    mainBar->DefineLabel("Save Level");
    mainBar->AddButton("SaveLevel", EditorSaveLevel, nullptr);

    mainBar->AddSeparator("spaces");

    mainBar->DefineLabel("Create New Space");
    mainBar->DefineHelpMessage("Creates a blank space from scratch");
    mainBar->AddButton("NewSpace", EditorCreateSpace, ENGINE);

    mainBar->DefineLabel("Load Space");
    mainBar->AddButton("LoadSpace", EditorOpenSpace, ENGINE);

    mainBar->AddSeparator("PIESTUFF");
    mainBar->DefineLabel("Play In Editor");
    mainBar->AddButton("PIE", EditorPlayLevel, ENGINE);

    mainBar->AddSeparator("Gizmos");

    mainBar->DefineLabel("Gizmo Mode");
    mainBar->AddButton("GizmoMode", nullptr, nullptr);
    GizmoType cMode = GIZMO_EDITOR->GetGizmoMode();

    mainBar->DefineKeyShortcut("CTRL+q");
    mainBar->AddButton("Translation", SwitchGizmoTranslation, nullptr);

    mainBar->DefineKeyShortcut("CTRL+w");
    mainBar->AddButton("Scale", SwitchGizmoScale, nullptr);

    mainBar->DefineKeyShortcut("CTRL+r");
    mainBar->AddButton("Rotation", SwitchGizmoRotation, nullptr);

    mainBar->AddButton("Options", nullptr, nullptr);
    

    mainBar->DefineLabel("Snapping");
    mainBar->AddVarRW("Snapping", AntTweak::TW_TYPE_BOOLCPP,
      &(GIZMO_EDITOR->m_useSnapping));


    mainBar->DefineLabel("Snap Distance");
    mainBar->AddVarRW("SnapDistance", AntTweak::TW_TYPE_FLOAT,
      &(GIZMO_EDITOR->m_snapDistance));

    if (cMode == Translation)
    {
      //mainBar->AddVarRW("World", AntTweak::TW_TYPE_BOOLCPP,
      //  &(GIZMO_EDITOR->m_world));
    }

    if (cMode == Scale)
    {
      mainBar->DefineLabel("Dampening");
      mainBar->AddVarRW("ScaleDampening", AntTweak::TW_TYPE_FLOAT,
        &(GIZMO_EDITOR->m_scaleDampening));
    }

    


  }

  void Engine::OpenEditor()
  {
    // Create the main bar and get a handle to it
    
    ENGINE->m_editorAcitve = true;
    AntTweak::TBar* mainBar = ATWEAK->CreateBar("Editor");
    EngineBar = mainBar->self;

    UpdateEditorWindow();
  }

}