/*****************************************************************
Filename: Handle.h
*****************************************************************/

#ifndef OBJ_HANDLE_H
#define OBJ_HANDLE_H

namespace Framework
{

  class Handle
  {
  public:



    unsigned m_index : 16;
    unsigned m_counter : 16;

    static const Handle null;
  };

}



#endif
