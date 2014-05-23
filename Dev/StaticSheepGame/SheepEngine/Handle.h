/*****************************************************************
Filename: Handle.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef OBJ_HANDLE_H
#define OBJ_HANDLE_H

namespace Framework
{

  class Handle
  {
  public:
    Handle();
    Handle(unsigned index, unsigned counter);
    Handle(unsigned handle); // conversion

    inline operator unsigned(void) const;

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