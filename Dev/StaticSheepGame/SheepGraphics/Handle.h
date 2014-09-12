#pragma once

#include "DataTypes.h"
//#include "..\Tools\Resource.h"

namespace DirectSheep
{

class Handle : public Tools::Resource
{
  public:
    Handle();

    oType GetType(void) const;

    int operator*(void) const;
    operator bool(void) const;
    bool operator==(const Handle& rhs) const;
    bool operator!=(const Handle& rhs) const;
    bool operator<(const Handle& rhs) const; // For STL comparisons

  private:
    Handle(oType type, int index);

    oType type;
    int index;

    friend class RenderContext;
    friend class GraphicsManager;
};

} //namespace Graphics
