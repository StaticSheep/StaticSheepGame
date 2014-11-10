#pragma once
#include "Vertices.h"
namespace DirectSheep
{
  struct PositionVertex {
    Vec3 position;
  };

  struct PositionTextureVertex
  {
    Vec3 position;
    Vec2 texcoords;
  };
}
