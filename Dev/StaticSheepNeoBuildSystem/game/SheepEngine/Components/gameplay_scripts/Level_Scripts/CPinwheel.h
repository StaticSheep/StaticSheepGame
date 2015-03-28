#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
  class Pinwheel : public GameComponent
  {
  public:
    Pinwheel();
    ~Pinwheel();
    void LogicUpdate(float dt);
    void Initialize();
    void Remove();

    //member variables
    Handle gTransfrom;
    Handle gCollider;

    bool moveLeft;
    Vec3D velocity;
    int moveFactor;

    int frameSkip;
  };
}