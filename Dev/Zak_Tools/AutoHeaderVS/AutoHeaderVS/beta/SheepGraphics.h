/*****************************************************************
Filename: GraphicsInterface.h
*****************************************************************/

#pragma once

#include "System.h"
#include <wtypes.h>

namespace Framework
{
  class Camera;

	class SheepGraphics : public ISystem
	{
	public:
    // Update per frame



		// Sheep graphics interface constructor
		// Sheep graphics interface deconstructor



	private:

    // Initialization call






  public:

    Camera* CurrentCamera;
    HWND _HWnd;
    int _ScreenWidth;
    int _ScreenHeight;

	};

	extern SheepGraphics* GRAPHICS;
}


