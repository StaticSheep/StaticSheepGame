/*****************************************************************
Filename: Beam.cpp
Project: GAM250
Author(s): Scott Nelson(primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include <vector>
#include "types/vectors/Vec4.h"
namespace Framework
{
  struct Beam
  {
    Beam() : beamColor(1,1,1,1), direction(1,0), numBeams(0), texID(0) {}

    // lenght of each subbeam
    std::vector<float> beamLengths;

    // Total number of subbeams to draw
    int numBeams;

    // Total width of beam
    float totalWidth;

    // start of beam
    Vec3 origin;

    // Orientation of beam
    Vec2 direction;

    Vec4 beamColor;

    // Texture of beam
    int texID;
  };
}