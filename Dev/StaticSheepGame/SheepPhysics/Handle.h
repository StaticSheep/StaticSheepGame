/*****************************************************************
Filename: Handle.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef OBJ_HANDLE_H
#define OBJ_HANDLE_H

namespace SheepFizz
{

  class Handle
  {
  public:
    PHY_API Handle();
    PHY_API Handle(unsigned index, unsigned counter);
    PHY_API Handle(unsigned handle); // conversion

    //static void Serialize(File& file, Variable var);

    PHY_API inline operator unsigned(void) const;

    unsigned m_index : 16;
    unsigned m_counter : 16;

    static const Handle null;
  };

  Handle::operator unsigned( void ) const
  {
    return m_counter << 16 | m_index;
  }

}



#endif