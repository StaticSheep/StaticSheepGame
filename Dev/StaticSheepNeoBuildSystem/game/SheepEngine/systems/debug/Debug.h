/*****************************************************************
Filename: Debug.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "systems/System.h"

namespace Framework
{
  // prototypes for defining pointers
  struct DebugAudio;
  struct DebugGraphics;
  struct DebugFramerate;
  struct DebugInput;
  struct DebugPhysics;

  enum DebugState
  {
    DEBUG_AUDIO = 1,
    DEBUG_GRAPHICS,
    DEBUG_PERFORMANCE,
    DEBUG_INPUT,
    DEBUG_PHYSICS
  };

  // Holds performance information to draw to the screen.
  struct PerformanceData
  {
    float pos[16];
    float width[16];
    Vec4 color[16];
    int systemCount;
    const char* names[16];
    float times[16];
  };

  class Debug : public ISystem
  {
  public:

    Debug();
    ~Debug();

    virtual std::string GetName() {return "SheepDebug";};

    void ReceiveMessage(Message& msg);

    void Initialize(void);
    void Update(float dt);

  private:

    int GetState();
    void FormatString(int);
    void DrawPerformance();
    std::string string;

    int currentState;

    /*---------- Debug Data Structs ----------*/

    DebugAudio*     audio;
    DebugGraphics*  graphics;
    DebugFramerate* framerate;
    DebugInput*     input;
    DebugPhysics*   physics;

  };
}