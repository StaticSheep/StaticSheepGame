/*****************************************************************
Filename: System.h
Project: 

*****************************************************************/

// Can we use this?
#pragma once

#include <string>
#include "Message.h"

namespace Framework
{

	// Similar to what Chris Peters does, this is a purely virtual base class
	// for all systems and interfaces.
	class ISystem
	{
	public:
    // Register any system components with this function

		// Initialization call for the system

		// All system can be updated every frame through this call

		// Systems receive and handle messages in different ways

		// Systems can have names too!


		// Virtual deconstructer
		
	};
}
