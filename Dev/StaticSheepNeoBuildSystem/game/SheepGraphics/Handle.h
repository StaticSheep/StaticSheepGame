/*****************************************************************
Filename: Handle.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "include/SheepGraphics/api.h"

#include "DataTypes.h"
//#include "..\Tools\Resource.h"

namespace DirectSheep
{

class Handle
{
  public:
   Handle();

   ObjectType GetType(void) const;
   int GetIndex(void) const;

    int operator*(void) const;
    operator bool(void) const;
   bool operator==(const Handle& rhs) const;
   bool operator!=(const Handle& rhs) const;
   bool operator<(const Handle& rhs) const; // For STL comparisons
   Handle(ObjectType type, int index);
   Handle(ObjectType type, void* ptr);
  private:

    ObjectType type;

    union
    {
      int index;
      void* ptr;
    };

    friend class RenderContext;
    friend class Tex2D;
};

}
