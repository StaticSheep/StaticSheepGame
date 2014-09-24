/*****************************************************************
Filename: EngineTweak.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "AntTweakModule.h"
#include <windows.h>
#include <commdlg.h>
#include <iostream>



namespace Framework
{

  static Handle EngineBar;

  static void EditorCreateSpace(void* clientData)
  {
    GameSpace* space = ENGINE->CreateSpace("NewSpace");
    space->Tweak();
  }

  static void EditorOpenSpace(void* clientData)
  {
    OPENFILENAME ofn;
    char szFile[100];

    ZeroMemory( &ofn , sizeof( ofn));
    ofn.lStructSize = sizeof ( ofn );
    ofn.hwndOwner = NULL  ;
    ofn.lpstrFile = szFile ;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof( szFile );
    ofn.lpstrFilter = "Sheep Spaces\0*.level\0All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex =1;
    ofn.lpstrFileTitle = NULL ;
    ofn.nMaxFileTitle = 0 ;
    ofn.lpstrInitialDir=NULL ;
    ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
    GetOpenFileName( &ofn );

    if (szFile[0] == 0)
      return;

    GameSpace* sp = FACTORY->LoadSpace(szFile);
    sp->Tweak();
  }

  static void EditorLoadLevel(void* clienData)
  {

  }

  static void EditorPlayLevel(void* clientData)
  {

  }

  void Engine::OpenEditor()
  {
    // Create the main bar and get a handle to it
    AntTweak::TBar* mainBar = ATWEAK->CreateBar("Editor");
    EngineBar = mainBar->self;

    mainBar->DefineLabel("Create New Space");
    mainBar->DefineHelpMessage("Creates a blank space from scratch");
    mainBar->AddButton("NewSpace", EditorCreateSpace, ENGINE);

    mainBar->DefineLabel("Load Space");
    mainBar->AddButton("LoadSpace", EditorOpenSpace, ENGINE);
  }

}