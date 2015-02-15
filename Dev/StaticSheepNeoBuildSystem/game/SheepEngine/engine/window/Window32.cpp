/*****************************************************************
Filename: Window.cpp
Project: 
Author(s): Scott Nelson (Primary)
           Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "engine/window/Window32.h"
#include "systems/input/Input.h"

#include <windows.h>
#include <windowsx.h>
#include "AntTweakBar.h"
#include "systems/anttweak/AntTweakModule.h"


static void GetDesktopResolution(int& horizontal, int& vertical)
{
  RECT desktop;
  // Get a handle to the desktop window
  const HWND hDesktop = GetDesktopWindow();
  // Get the size of screen to the variable desktop
  GetWindowRect(hDesktop, &desktop);
  // The top left corner will have coordinates (0,0)
  // and the bottom right corner will have coordinates
  // (horizontal, vertical)
  horizontal = desktop.right;
  vertical = desktop.bottom;
}

namespace Framework
{

  bool WINDOW_ACTIVE = true;
  static bool localATValid = false;

  LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  void SheepWindow::MakeWindow(void* hInstance, int nCmdShow, bool fullScreen)
  {
    WNDCLASSEX wcex;                                // Struct containing Window class data
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));          // Null out unused parameters
    wcex.cbSize = sizeof(WNDCLASSEX);               // Size in memory of class type
    wcex.style = CS_HREDRAW | CS_VREDRAW;           // Updates window when moved
    wcex.lpfnWndProc = WindowProc;                  // Function to be used for message processing
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = *(HINSTANCE*)hInstance;                     // Handle for the window instane being used by winmain
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // Cursor for window
    wcex.lpszMenuName = nullptr;                    // Menus are not being used
    wcex.lpszClassName = "SheepWindow";               // Name of class

    ErrorIf(!RegisterClassEx(&wcex), "Window", "Window class failed to register!");

    // Create Window
    width = Config::desiredWidth;
    height = Config::desiredHeight;
    if (fullScreen)
    {
      GetDesktopResolution(width, height);
    }
    
    RECT rc = {0, 0, width, height};                     // Defines rectangle dimensions for window
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE); // Takes borders into considerations for window size

    // Creates a window using registered classes params along with RECT size
    Handle = CreateWindow("SheepWindow",         // Name of window class
      "Sheep Engine 2015",      // Text on top bar
      WS_OVERLAPPEDWINDOW, // Type of window
      CW_USEDEFAULT, 
      CW_USEDEFAULT, 
      rc.right - rc.left,  // Width of window
      rc.bottom - rc.top,  // Height of Window
      nullptr, 
      nullptr, 
      *(HINSTANCE*)hInstance,           // Instance of window to be configured
      nullptr);

    ErrorIf(!Handle, "Window", "Window failed to create!");
    //SetWindowPos(ENGINE->Window->GetHandle(), NULL, 0, 0, width, height, 0);

    //ShowWindow(Handle, nCmdShow);              // Display window
  }

  LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    PAINTSTRUCT ps;                         // Data to redraw window if moved
    HDC hdc;                                // Hande to a Device Context
    MSG msg;
    DWORD windowSize;
    WORD width;
    WORD height;

#if USE_ANTTWEAKBAR
    if (TwEventWin(hWnd, message, wParam, lParam))
    {
      return 0; // Event has been handled by AntTweakBar
    }
    
      
#endif

    switch( message )                       // Check message
    {
    case WM_PAINT:                      // Window has moved
      hdc = BeginPaint( hWnd, &ps );
      EndPaint( hWnd, &ps );
      break;

    case WM_SIZE:
      windowSize = lParam;
      width = LOWORD(windowSize);
      height = HIWORD(windowSize);
      if (ENGINE != nullptr)
      {
        ENGINE->SystemMessage(ResizeMessage(width, height));
      }
      
      break;

    case WM_DESTROY:                    // Window has been requested to close
      ENGINE->Quit();
      PostQuitMessage( 0 );
      break;

    case WM_ACTIVATE:
      if (wParam == WA_INACTIVE && !ENGINE->m_editorActive)
      {
        ENGINE->SystemMessage(Message(Message::WindowMinimize));

        ShowWindow(hWnd, SW_MINIMIZE);

        WINDOW_ACTIVE = false;
      }
      else if ((wParam == WA_CLICKACTIVE || wParam == WA_ACTIVE) &&
        !ENGINE->m_editorActive)
      {
        if (!ENGINE->PlayingInEditor())
        {
          ENGINE->SystemMessage(Message(Message::WindowRestore));
          ShowCursor(false);
        }

        ShowWindow(hWnd, SW_RESTORE);
        WINDOW_ACTIVE = true;
      }
      break;

/*****************************************************************************/
//    Mouse Events
/*****************************************************************************/
    case WM_MOUSEMOVE:

    case WM_LBUTTONDOWN:

    case WM_LBUTTONUP:

    case WM_LBUTTONDBLCLK:

    case WM_RBUTTONDOWN:

    case WM_RBUTTONUP:

    case WM_RBUTTONDBLCLK:

    case WM_MBUTTONDOWN:

    case WM_MBUTTONUP:

    case WM_MBUTTONDBLCLK:

      TranslateMessage(&msg);

      break;

/*****************************************************************************/
//    Mouse Events End
/*****************************************************************************/

    default:
      return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
  }

  void SheepWindow::Update()
  {
    MSG msg = {0}; // msg struct holds return of peek message

    //if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Query message queue
    //{
    //  TranslateMessage(&msg);                  // Translate return
    //  DispatchMessage(&msg);                   // Dispatch
    //}
  }

  HWND SheepWindow::GetHandle()
  {
    return Handle;
  }

  int SheepWindow::GetWidth()
  {
    return width;
  }

  int SheepWindow::GetHeight()
  {
    return height;
  }
}
