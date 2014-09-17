/*****************************************************************
Filename: Handle.h
*****************************************************************/

#pragma once

namespace SheepFizz
{

  class Handle
  {
  public:
    PHY_API Handle();
    PHY_API Handle(unsigned index, unsigned counter);
    PHY_API Handle(unsigned handle); // conversion


    PHY_API inline operator unsigned(void) const;

    unsigned m_index : 16;
    unsigned m_counter : 16;

    static const Handle null;
  };

}

