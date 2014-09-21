// ------------------------------------------------------------------------------------
//  File     : GFXHandle.h
//  Author   : Kai-Hwa Yao
//  Copyright: All content © 2013-2014 DigiPen (USA) Corporation, all rights reserved.
// ------------------------------------------------------------------------------------

#pragma once

#include "GFXTypes.h"
#include "..\Tools\Resource.h"

namespace Graphics
{

class Handle : public Tools::Resource
{
  public:
    Handle();

    ObjectType GetType(void) const;

    int operator*(void) const;
    operator bool(void) const;
    bool operator==(const Handle& rhs) const;
    bool operator!=(const Handle& rhs) const;
    bool operator<(const Handle& rhs) const; // For STL comparisons

  private:
    Handle(ObjectType type, int index);

    ObjectType type;
    int index;

    friend class RenderContext;
    friend class GraphicsManager;
};

} //namespace Graphics
