/*****************************************************************
Filename: Handle.h
*****************************************************************/

#pragma once

#include "SheepPhysics/api.h"

namespace SheepFizz
{

  class Handle
  {
  public:
    PHY_API Handle();
    PHY_API Handle(unsigned index, unsigned counter);
    PHY_API Handle(unsigned handle); // conversion


    PHY_API operator unsigned(void) const { return m_counter << 16 | m_index; }


    unsigned m_index : 16;
    unsigned m_counter : 16;

    static const Handle null;
  };

}

