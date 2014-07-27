/*****************************************************************
Filename: GraphicsInterface.h
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"
#include <wtypes.h>

namespace Framework
{
  class Camera;

  enum Shaders
  {
    Basic = 0,
    Debug,
    NumberOfShaders
  };

	class SheepGraphics : public ISystem
	{
	public:
		// Sheep graphics interface constructor
		SheepGraphics();
		// Sheep graphics interface deconstructor
		~SheepGraphics();

		virtual std::string GetName() {return "SheepGraphics";};

    void SetWindwProperties(HWND hWnd, int screenWidth, int screenHeight);

		// Update per frame
		void Update(float dt);

	private:
		void Draw(void);

    // Initialization call
		void Initialize(void);

    bool InitGeometry();

    void LoadAssets();

    void LoadTexture(const std::string& filename);

    void SetupMatrices();

    bool LoadEffect(int index, const std::string& filename);

  public:

    Camera* CurrentCamera;
    HWND _HWnd;
    int _ScreenWidth;
    int _ScreenHeight;



	};

	extern SheepGraphics* GRAPHICS;
}

