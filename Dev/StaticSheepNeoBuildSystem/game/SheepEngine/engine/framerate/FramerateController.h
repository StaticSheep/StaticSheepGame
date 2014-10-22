/******************************************************************************
Filename: FramerateController.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include <vector>

#pragma once

namespace Framework
{

  class FramerateController;

  struct DebugSystem
  {
    float timeTaken;
    const char* name;
  };

  struct DebugFramerate
  {
    float currentFps;
    std::unordered_map<std::string, DebugSystem*> systems;
    float totalTime;
  };

	class FramerateController
	{
	public:

    // constructors deconstructors...
    FramerateController();
    FramerateController(double);
    ~FramerateController();
		// Initialization call for the system
		void Initialize(int index, const char* name);

    void* GetDebugData();

    // check if enough time has passed
    bool FramerateCheck(void);

    // debug check for system times
    void StartFrame(void);
    double EndFrame(const char*);

    // get the current delta time
    float GetDT();

    // get the current frames per second
    float GetCurrentFPS();

  private:

    void* _previousTime; // for frameratecheck()
    void* _previousSystemTime; // for checking systems
    void* _freq; // hardware frequency for calculating time
    double _dt; // current delta time
    double _currentfps; // how many fps we got last frame
    double _fps; // the fps we want

    DebugFramerate debug;

	};
}