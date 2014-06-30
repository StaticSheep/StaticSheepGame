/*****************************************************************
Filename: Generic.h
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{

  class Generic
  {
  public:
    EGenerics genericType;

    virtual void Initialize() = 0;

    Generic(EGenerics type) : genericType(type) {};
  protected:
    Generic();
  };

}