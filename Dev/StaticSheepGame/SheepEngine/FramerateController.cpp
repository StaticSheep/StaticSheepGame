
#include <Windows.h>

#define WILSONS_CONSTANT1 1000000000

namespace Framework
{

  FramerateController::FramerateController()
  {
    _previousTime = new LARGE_INTEGER; // MAKE SOME BIG FUCKING INTEGERS
    _previousSystemTime = new LARGE_INTEGER;
    _freq = new LARGE_INTEGER;
    _fps = 1.0 / 60.0; // default to 60 fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)_previousTime);
    // get the frequency
    QueryPerformanceFrequency((LARGE_INTEGER*)_freq);
  }

  FramerateController::FramerateController(double fps)
  {
    _previousTime = new LARGE_INTEGER; // MAKE SOME BIG FUCKING INTEGERS
    _previousSystemTime = new LARGE_INTEGER;
    _freq = new LARGE_INTEGER;
    _fps = 1.0 / fps; // get the set fps

    // get time now
    QueryPerformanceCounter((LARGE_INTEGER*)_previousTime);
    // get the frequency
    QueryPerformanceFrequency((LARGE_INTEGER*)_freq);
  }

  FramerateController::~FramerateController()
  {
    delete _freq;
    delete _previousSystemTime;
    delete _previousTime;
  }

  void FramerateController::Initialize(int index, const char* name)
  {
    debug.systems[name] = (new DebugSystem);
  }

  void* FramerateController::GetDebugData()
  {
    debug.currentFps = _currentfps;


    return (void*)&debug;
  }

  bool FramerateController::FramerateCheck()
  {
    LARGE_INTEGER currentTime, elapsedTime;

    QueryPerformanceCounter(&currentTime);
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)_previousTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1; //don't worry about this, shhhhh
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)_freq)->QuadPart;
  
    if((double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1 > _fps - (_fps / 1000.0))
    {
      ((LARGE_INTEGER*)_previousTime)->QuadPart += currentTime.QuadPart - ((LARGE_INTEGER*)_previousTime)->QuadPart;
      _dt = (double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1;
      _currentfps = 1.0 / _dt;
      return true;
    }

    return false;
  }

  void FramerateController::StartFrame()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)_previousSystemTime);
  }

  double FramerateController::EndFrame(const char* name)
  {
    LARGE_INTEGER currentTime, elapsedTime;

    QueryPerformanceCounter(&currentTime);
    elapsedTime.QuadPart = currentTime.QuadPart - ((LARGE_INTEGER*)_previousSystemTime)->QuadPart;
    elapsedTime.QuadPart *= WILSONS_CONSTANT1;
    elapsedTime.QuadPart /= ((LARGE_INTEGER*)_freq)->QuadPart;

    debug.systems[name]->timeTaken = (float)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1;
    
    return (double)elapsedTime.QuadPart / (double)WILSONS_CONSTANT1;
  }

  float FramerateController::GetDT()
  {
    return (float)_dt;
  }

  float FramerateController::GetCurrentFPS()
  {
    return (float)_currentfps;
  }

   
};