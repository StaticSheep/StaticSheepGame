/******************************************************************************
Filename: FramerateController.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include <Windows.h>

#define WILSONS_CONSTANT1 1000000000

namespace Framework
{


/*****************************************************************************/
/*!
  \brief
    Default constructor for the FramerateController class. Sets FPS to 60.
*/
/*****************************************************************************/
  FramerateController::FramerateController()
  {
    _previousTime = new LARGE_INTEGER; // MAKE SOME BIG GODDAMN INTEGERS
    _previousSystemTime = new LARGE_INTEGER;
    _freq = new LARGE_INTEGER;
    _fps = 1.0 / 60.0; // default to 60 fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)_previousTime);
    // get the frequency
    QueryPerformanceFrequency((LARGE_INTEGER*)_freq);
  }

/*****************************************************************************/
/*!
  \brief
    Non-Default constructor for the FramerateController class. Sets FPS to the
    passed in double.
*/
/*****************************************************************************/
  FramerateController::FramerateController(double fps)
  {
    _previousTime = new LARGE_INTEGER; // MAKE SOME BIG GODDAMN INTEGERS
    _previousSystemTime = new LARGE_INTEGER;
    _freq = new LARGE_INTEGER;
    _fps = 1.0 / fps; // get the set fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)_previousTime);
    // get the frequency of the hardware
    QueryPerformanceFrequency((LARGE_INTEGER*)_freq);
  }

/*****************************************************************************/
/*!
  \brief
    Destructor for Framerate, just get right of some dynamically allocated
    things.
*/
/*****************************************************************************/
  FramerateController::~FramerateController()
  {
    delete _freq;
    delete _previousSystemTime;
    delete _previousTime;
  }

/*****************************************************************************/
/*!
  \brief
    For setting up debug information about each system being added to the 
    engine. This is later used to test how long each system takes to update.
*/
/*****************************************************************************/
  void FramerateController::Initialize(int index, const char* name)
  {
    debug.systems[name] = (new DebugSystem);
  }

/*****************************************************************************/
/*!
  \brief
    Used by the Debug class to get the current fps.
*/
/*****************************************************************************/
  void* FramerateController::GetDebugData()
  {
    debug.currentFps = (float)_currentfps;
    return (void*)&debug;
  }

/*****************************************************************************/
/*!
  \brief
    Checks to see if enough time has passed since the last frame to continue
    another frame of update.
*/
/*****************************************************************************/
  bool FramerateController::FramerateCheck()
  {
    LARGE_INTEGER currentTime, elapsedTime;

    QueryPerformanceCounter(&currentTime);
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)_previousTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1; //don't worry about this, shhhhh
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)_freq)->QuadPart;
  
    //                                                          fps we want - a tiny bit
    //                                                          otherwise it'll go over a tiny bit...
    if((double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1 > _fps - (_fps / 1000.0))
    {
      ((LARGE_INTEGER*)_previousTime)->QuadPart += currentTime.QuadPart - ((LARGE_INTEGER*)_previousTime)->QuadPart;
      _dt = (double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1;
      _currentfps = 1.0 / _dt;
      return true;
    }

    return false;
  }

/*****************************************************************************/
/*!
  \brief
    Used at the start of a system/module, sets the start time.
*/
/*****************************************************************************/
  void FramerateController::StartFrame()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)_previousSystemTime);
  }

/*****************************************************************************/
/*!
  \brief
    Used at the end of a system/module, calculates how long the system/module
    took to update.
*/
/*****************************************************************************/
  double FramerateController::EndFrame(const char* name)
  {
    LARGE_INTEGER currentTime, elapsedTime;

    QueryPerformanceCounter(&currentTime);
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)_previousSystemTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1;
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)_freq)->QuadPart;

    debug.systems[name]->timeTaken = (float)(elapsedTime.QuadPart / (double)WILSONS_CONSTANT1);
    
    return (double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1;
  }

/*****************************************************************************/
/*!
  \brief
    Gets the accurate and current delta time.
*/
/*****************************************************************************/
  float FramerateController::GetDT()
  {
    return (float)_dt;
  }

/*****************************************************************************/
/*!
  \brief
    Gets the current fps of the system.
*/
/*****************************************************************************/
  float FramerateController::GetCurrentFPS()
  {
    return (float)_currentfps;
  }

   
} // end namespace
