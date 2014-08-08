/*****************************************************************
Filename: GraphicsInterface.h
Project: 
Author(s): Zachary Nawar

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
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
		void Update(float dt);

    virtual void RegisterComponents();

    virtual std::string GetName() {return "SheepGraphics";};

		// Sheep graphics interface constructor
		SheepGraphics();
		// Sheep graphics interface deconstructor
		~SheepGraphics();

    void SetPosition(float x, float y);
    void SetRotation(float theta);
    void SetSize(float x, float y);
    void SetTexture(int ID);
    void SetUseCamera(bool useCam);
    void DrawSprite(void);
    int GetTextureID(std::string& texture);

    //void StartFrame(void); //?
    void FinishFrame(void);

	private:

    // Initialization call
		void Initialize(void);

    void InitGeometry();

    void LoadAssets();

    void LoadTexture(const std::string& filename);

    void SetupMatrices();

		void Draw(void);

  public:

    Camera* CurrentCamera;
    HWND _HWnd;
    int _ScreenWidth;
    int _ScreenHeight;

	};

	extern SheepGraphics* GRAPHICS;
}
