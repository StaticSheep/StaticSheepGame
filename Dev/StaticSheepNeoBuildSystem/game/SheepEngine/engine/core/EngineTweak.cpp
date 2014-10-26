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
    char szFile[200];

    TCHAR Buffer[200];
    DWORD dwRet;

    dwRet = GetCurrentDirectory(200, Buffer);

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

    GameSpace* sp = FACTORY->LoadSpace(szFile);
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
    char szFile[100];

    TCHAR Buffer[128];
    DWORD dwRet;

    dwRet = GetCurrentDirectory(128, Buffer);

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

    FACTORY->LoadLevel(szFile, TweakSpaceCB);
  }

  static void EditorSaveLevel(void* clienData)
  {
    OPENFILENAME ofn;
    char szFile[100];

    TCHAR Buffer[128];
    DWORD dwRet;

    dwRet = GetCurrentDirectory(128, Buffer);

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
    ofn.lpstrInitialDir="content\\data\\spaces";
    ofn.Flags = OFN_PATHMUSTEXIST;
    GetSaveFileName(&ofn);

    if (szFile[0] == 0)
      return;

    SetCurrentDirectory(Buffer);

    FACTORY->SaveLevel(szFile);
  }

  static void EditorPlayLevel(void* clientData)
  {

  }

  void Engine::OpenEditor()
  {
    // Create the main bar and get a handle to it
    
    AntTweak::TBar* mainBar = ATWEAK->CreateBar("Editor");
    EngineBar = mainBar->self;

    mainBar->DefineLabel("Load Level");
    mainBar->AddButton("LoadLevel", EditorLoadLevel, nullptr);

    mainBar->DefineLabel("Save Level");
    mainBar->AddButton("SaveLevel", EditorSaveLevel, nullptr);

    mainBar->AddSeparator("Spaceshit");

    mainBar->DefineLabel("Create New Space");
    mainBar->DefineHelpMessage("Creates a blank space from scratch");
    mainBar->AddButton("NewSpace", EditorCreateSpace, ENGINE);

    mainBar->DefineLabel("Load Space");
    mainBar->AddButton("LoadSpace", EditorOpenSpace, ENGINE);
  }

}