/*****************************************************************
Filename: Window.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once


#include <wtypes.h>

namespace Framework
{

  class SheepWindow
  {
  public:
    SheepWindow() : Handle(NULL) {};
    void MakeWindow(HINSTANCE hInstance, int nCmdShow);
    void Update();
    HWND GetHandle();
    int width, height;
  protected:
    HWND Handle;
  };


}

