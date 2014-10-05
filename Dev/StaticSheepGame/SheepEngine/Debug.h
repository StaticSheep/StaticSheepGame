#include "System.h"
#include <vector>

namespace Framework
{
  struct DebugAudio;
  struct DebugGraphics;
  struct DebugFramerate;

  enum DebugState
  {
    DEBUG_AUDIO = 1,
    DEBUG_GRAPHICS,
    DEBUG_PERFORMANCE
  };

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

    void ReceiveMessage(Message msg);

    void Initialize(void);
    void Update(float dt);

    //void* GetDebugData(void);

  private:

    int GetState();
    void FormatString(int);
    void DrawPerformance();
    std::string string;

    int currentState;

    DebugAudio*     audio;
    DebugGraphics*  graphics;
    DebugFramerate* framerate;

  };
}