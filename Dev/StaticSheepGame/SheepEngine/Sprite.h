#include "CTransform.h"
namespace Framework
{
  class Transform;

  class Sprite : public GameComponent
  {
  public:

    Sprite();
    ~Sprite();

    virtual void Initialize();

    Vec2 Size;

    Transform *transform;

    std::string SpriteName;

    void Draw();
  };
}