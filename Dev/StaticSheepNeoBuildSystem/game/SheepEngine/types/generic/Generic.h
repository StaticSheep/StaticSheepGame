/*****************************************************************
Filename: Generic.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{

  class GameSpace;

  class Generic
  {
  public:
    EGenerics genericType;

    virtual void Initialize() {};

    Generic(EGenerics type) : genericType(type), space(nullptr), self() {};
    Generic() : genericType(eAbstract), space(nullptr), self() {};

    // Self handle
    Handle self;

    // Pointer to the space which the generic
    GameSpace* space;
  };

  struct GenericLookup
  {
    Handle self;
    GameSpace* space;
  };

}