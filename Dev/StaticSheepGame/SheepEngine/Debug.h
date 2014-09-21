#include "System.h"
#include <vector>

struct DebugAudio;

namespace Framework
{
  class Debug : public ISystem
  {
  public:

    Debug();
    ~Debug();

    void Initialize(void);
    void Update(float dt);

    void* GetDebugData(void);

  private:

    DebugAudio* audio;

  };
}