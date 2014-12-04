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
  extern bool WINDOW_ACTIVE;
  class SheepWindow
  {
  public:
    void MakeWindow(void* hInstance, int nCmdShow);
    void Update();
    HWND GetHandle();

    int GetWidth();
    int GetHeight();
    
  protected:
    HWND Handle;
    int width, height;
    
  };


}

