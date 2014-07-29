#include "Sprite.h"
#include "graphics\sheep_graphics.h"
namespace Framework
{

  struct Vec4
  {
    float r;
    float g;
    float b;
    float a;
  };

  Sprite::Sprite()
  {
    transform = NULL;
  }

  Sprite::~Sprite()
  {
  }

  void Sprite::Initialize()
  {
    //TODO not sure if we have a GetOwner()->has working
  }

  void Sprite::Draw()
  {
    //DirectSheep::GFX_Draw(Size, transform->position, transform->rotation);
  }
}