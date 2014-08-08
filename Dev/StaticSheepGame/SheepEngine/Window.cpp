/*****************************************************************
Filename: Window.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Window.h"

#include <windows.h>
#include <windowsx.h>

namespace Framework
{
  LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  void SheepWindow::MakeWindow(HINSTANCE hInstance, int nCmdShow)
  {
    WNDCLASSEX wcex;                                // Struct containing Window class data
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));          // Null out unused parameters
    wcex.cbSize = sizeof(WNDCLASSEX);               // Size in memory of class type
    wcex.style = CS_HREDRAW | CS_VREDRAW;           // Updates window when moved
    wcex.lpfnWndProc = WindowProc;                  // Function to be used for message processing
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;                     // Handle for the window instane being used by winmain
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // Cursor for window
    wcex.lpszMenuName = nullptr;                    // Menus are not being used
    wcex.lpszClassName = "SheepWindow";               // Name of class

    ErrorIf(!RegisterClassEx(&wcex), "Window", "Window class failed to register!");

    // Create Window
    width = 800;
    height = 600;
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
      hInstance,           // Instance of window to be configured
      nullptr);

    ErrorIf(!Handle, "Window", "Window failed to create!");

    ShowWindow(Handle, nCmdShow);              // Display window
  }

  LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
    PAINTSTRUCT ps;                         // Data to redraw window if moved
    HDC hdc;                                // Hande to a Device Context

    switch( message )                       // Check message
    {
    case WM_PAINT:                      // Window has moved
      hdc = BeginPaint( hWnd, &ps );
      EndPaint( hWnd, &ps );
      break;

    case WM_DESTROY:                    // Window has been requested to close
      ENGINE->Quit();
      PostQuitMessage( 0 );
      break;

    default:
      return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
  }

  void SheepWindow::Update()
  {
    MSG msg = {0}; // msg struct holds return of peek message

    if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Query message queue
    {
      TranslateMessage(&msg);                  // Translate return
      DispatchMessage(&msg);                   // Dispatch
    }
  }

  HWND SheepWindow::GetHandle()
  {
    return Handle;
  }
}