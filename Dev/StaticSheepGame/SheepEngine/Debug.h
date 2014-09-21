#include "System.h"
#include <vector>




namespace Framework
{
  struct DebugAudio;
  struct DebugGraphics;

  class Debug : public ISystem
  {
  public:

    Debug();
    ~Debug();

    void Initialize(void);
    void Update(float dt);

    //void* GetDebugData(void);

  private:

    DebugAudio*     audio;
    DebugGraphics*  graphics;

  };
}