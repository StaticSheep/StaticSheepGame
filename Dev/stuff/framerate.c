/*****************************************************************
Filename: framerate.c
Project: Sol:Descent
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include "framerate.h"
#include "trace.h"
#include <Windows.h>

/////////////////////////////////////////////////////////////////
////				PUBLIC VARIABLES GO HERE				/////
/////////////////////////////////////////////////////////////////

LARGE_INTEGER lastFrame;
double framesPerSec;
int frameRate;

/////////////////////////////////////////////////////////////////
////				PUBLIC FUNCTIONS GO HERE				/////
/////////////////////////////////////////////////////////////////

/// <summary>
/// Initializes the framerate controller
/// </summary>
/// <returns></returns>
NOVA_API void FrameRate_Init()
{
  Trace_Message(Verbose, "Framerate controller initialized");
  FrameRate_SetRate(63);
  FrameRate_SetLastFrame();
}

/// <summary>
/// Sets the framerate
/// </summary>
/// <param name="rate">Frames per second</param>
/// <returns></returns>
NOVA_API void FrameRate_SetRate(int rate)
{
  Trace_Message(Debug, "Framerate set to %dfps", rate);
  frameRate = rate;
  framesPerSec = 1 / (double)rate;
}

// Calculates the time since the given time
double timeSince(LARGE_INTEGER *since)
{
  LARGE_INTEGER now, frequency;
  QueryPerformanceCounter(&now);
  QueryPerformanceFrequency(&frequency);

  return ((double)(now.QuadPart - since->QuadPart) / (double)frequency.QuadPart);
}

/// <summary>
/// Checks to see if enough time has passed since the last frame
/// </summary>
/// <returns>0 if not enough time, or Deltatime</returns>
NOVA_API double FrameRate_Check()
{
  double time = timeSince(&lastFrame);
  if ( time > framesPerSec)
  {
    return framesPerSec;
  }
  return 0;
}

/// <summary>
/// Sets the last frame to current time
/// </summary>
/// <returns></returns>
NOVA_API void FrameRate_SetLastFrame()
{
  QueryPerformanceCounter(&lastFrame);
}