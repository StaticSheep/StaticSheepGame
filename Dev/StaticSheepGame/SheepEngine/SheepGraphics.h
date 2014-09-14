/*****************************************************************
Filename: GraphicsInterface.h
Project: 
Author(s): Zachary Nawar

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"


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
    void SetColor(Vec4 Color);
    void SetUseCamera(bool useCam);
    void DrawSprite(void);
    int GetTextureID(std::string& texture);
    void DrawSpriteText(const char * text, float size, const char * font);

    //void StartFrame(void); //?
    void FinishFrame(void);

    bool AntTweakBarLoaded();

    void* GetDevice(void);

    static void DeviceInit(void);

	private:

    // Initialization call
		void Initialize(void);

    void InitGeometry();

    void LoadAssets();

    void LoadTexture(const std::string& filename);

		void Draw(void);

  public:

    Camera* CurrentCamera;
    int _ScreenWidth;
    int _ScreenHeight;

	};

	extern SheepGraphics* GRAPHICS;
}

