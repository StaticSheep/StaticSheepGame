/*****************************************************************
Filename: FramerateController.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "engine/core/Engine.h"
#include <Windows.h>

// needed to multiply numbers, since we are dividing by a lot and we don't want
// to lose resolution. Also for getting miliseconds and not microseconds
#define WILSONS_CONSTANT1 1000000000

namespace Framework
{
  // defaults to 60 frames per second
  FramerateController::FramerateController()
  {
    frameTime = new LARGE_INTEGER; // MAKE SOME BIG GODDAMN INTEGERS
    systemTime = new LARGE_INTEGER;
    frequency = new LARGE_INTEGER;
    fps = 1.0 / 60.0; // default to 60 fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)frameTime);
    // get the frequency
    QueryPerformanceFrequency((LARGE_INTEGER*)frequency);
  }

  // Sets fps to what is passed in
  FramerateController::FramerateController(double fps)
  {
    frameTime = new LARGE_INTEGER; // MAKE SOME BIG GODDAMN INTEGERS
    systemTime = new LARGE_INTEGER;
    frequency = new LARGE_INTEGER;
    fps = 1.0 / fps; // get the set fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)frameTime);
    // get the frequency of the hardware
    QueryPerformanceFrequency((LARGE_INTEGER*)frequency);
  }

  // dtor
  FramerateController::~FramerateController()
  {
    for (auto it = debug.systems.begin(); it != debug.systems.end(); ++it)
    {
      delete it->second;
      it->second = 0;
    }

    delete frequency;
    delete systemTime;
    delete frameTime;

  }

  
  void FramerateController::Initialize(void)
  {
    for (unsigned int i = 0; i < ENGINE->m_systems.size(); ++i)
    {
      debug.systems[ENGINE->m_systems[i]->GetName()] = (new DebugSystem);
    }
  }

  // Gets debug data
  void* FramerateController::GetDebugData()
  {
    debug.currentFps = (float)currentFps;
    return (void*)&debug;
  }

  // returns true if enough time has passed, returns false if not
  bool FramerateController::FramerateCheck()
  {
    LARGE_INTEGER currentTime, elapsedTime;

    // get the current time
    QueryPerformanceCounter(&currentTime);

    // check how much time has elapsed since the last frame
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)frameTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1; // for resolution of the number
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)frequency)->QuadPart;

    if((double)elapsedTime.QuadPart / WILSONS_CONSTANT1 > fps)
    {
      ((LARGE_INTEGER*)frameTime)->QuadPart = currentTime.QuadPart;
      dt = (double)elapsedTime.QuadPart / WILSONS_CONSTANT1;
      currentFps = 1.0 / dt;
      debug.totalTime = 0.0;
      return true;
    }

    return false;
  }

  // For checking system update times
  void FramerateController::StartFrame()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)systemTime);
  }

  // Calculates how long a system took to update
  double FramerateController::EndFrame(const char* name)
  {
    LARGE_INTEGER currentTime, elapsedTime;

    QueryPerformanceCounter(&currentTime);
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)systemTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1;
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)frequency)->QuadPart;

    debug.systems[name]->timeTaken = (float)(elapsedTime.QuadPart / (double)WILSONS_CONSTANT1);
    debug.totalTime += debug.systems[name]->timeTaken;
    
    return (double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1;
  }

  float FramerateController::GetDT()
  {
    return 0.0167f; // deterministic dt
  }

  // gets the current frames per second
  float FramerateController::GetCurrentFPS()
  {
    return (float)currentFps;
  }

} // end namespace
