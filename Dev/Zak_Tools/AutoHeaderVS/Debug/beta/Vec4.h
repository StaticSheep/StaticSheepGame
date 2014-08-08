/*****************************************************************
Filename: Vec4.h
Project: 

*****************************************************************/

#pragma once

namespace Framework
{
  class Vec4
  {
  public:
    {
      X = _x;
      Y = _y;
      Z = _z;
      A = _a;
    }


    union
    {
      float X;
      float R;
    };

    union
    {
      float Y;
      float G;
    };

    union
    {
      float Z;
      float B;
    };

    float A;
  };
}
