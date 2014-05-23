/*****************************************************************
Filename: GraphicsInterface.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "System.h"

namespace Framework
{
	class SheepGraphics : public ISystem
	{
	public:
		// Sheep graphics interface constructor
		SheepGraphics();
		// Sheep graphics interface deconstructor
		~SheepGraphics();

		virtual std::string GetName() {return "SheepGraphics";};

		// Initialization call
		void Initialize(void);

		// Update per frame
		void Update(float dt);

	private:
		void Draw(void);
	};

	extern SheepGraphics* GRAPHICS;
}