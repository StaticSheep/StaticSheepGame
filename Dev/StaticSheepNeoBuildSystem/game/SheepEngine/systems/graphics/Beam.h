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
    Beam() : m_beamCol(1,1,1,1), m_dirVec(1,0), m_numBeams(0), m_texID(0) {}

    // lenght of each subbeam
    std::vector<float> m_lengths;

    // Total number of subbeams to draw
    int m_numBeams;

    // Total width of beam
    float m_totalWidth;

    // start of beam
    Vec3 m_origin;

    // Orientation of beam
    Vec2 m_dirVec;

    Vec4 m_beamCol;

    // Texture of beam
    int m_texID;
  };
}