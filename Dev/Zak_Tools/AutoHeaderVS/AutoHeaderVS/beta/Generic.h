/*****************************************************************
Filename: Generic.h
*****************************************************************/

#pragma once

namespace Framework
{

  class GameSpace;

  class Generic
  {
  public:
    EGenerics genericType;



    // Self handle
    Handle self;

    // Pointer to the space which the generic
    GameSpace* space;
  };

}
