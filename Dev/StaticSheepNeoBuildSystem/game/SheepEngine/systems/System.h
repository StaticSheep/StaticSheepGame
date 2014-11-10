/*****************************************************************
Filename: System.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

// Can we use this?
#pragma once

#include <string>
#include "types/message/Message.h"

namespace Framework
{

	// Similar to what Chris Peters does, this is a purely virtual base class
	// for all systems and interfaces.
	class ISystem
	{
	public:
    // Register any system components with this function
    virtual void RegisterComponents() {};

		// Initialization call for the system
		virtual void Initialize() {};

		// All system can be updated every frame through this call
		virtual void Update(float dt) = 0;

		// Systems receive and handle messages in different ways
		virtual void ReceiveMessage(Message& msg) {};

		// Systems can have names too!
		virtual std::string GetName() = 0;

    virtual void Shutdown() {};

    // Function to return debug data from the systems
    virtual const void* GetDebugData(void) {return nullptr;};

		// Virtual deconstructer
		virtual ~ISystem() {};

    ISystem() {};
	};
}