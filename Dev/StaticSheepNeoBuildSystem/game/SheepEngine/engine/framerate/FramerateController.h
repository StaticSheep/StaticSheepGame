/*****************************************************************
Filename: FramerateController.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include <vector>

#pragma once

namespace Framework
{

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
		void Initialize();

    void* GetDebugData();

    // check if enough time has passed
    bool FramerateCheck(void);

    // debug check for system times
    void StartFrame(void);
    double EndFrame(const char*);

    void EndMainLoop();

    // get the current delta time
    float GetDT();

    // get the current frames per second
    float GetCurrentFPS();

  private:

    void* frameTime; // for frameratecheck()
    void* systemTime; // for checking systems
    void* frequency; // hardware frequency for calculating time
    double dt; // current delta time
    double currentFps; // how many fps we got last frame
    double fps; // the fps we want

    DebugFramerate debug;

	};
}