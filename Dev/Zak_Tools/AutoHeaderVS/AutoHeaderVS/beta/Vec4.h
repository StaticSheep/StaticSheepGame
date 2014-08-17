/*****************************************************************
Filename: Vec4.h
*****************************************************************/

#pragma once

namespace Framework
{
  class Vec4
  {


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
