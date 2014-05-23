/*****************************************************************
Filename: Handle.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Handle.h"

namespace Framework
{
  const Handle Handle::null;

  Handle::Handle( )
    : m_index( -1 )
    , m_counter( -1 )
  {
  }

  Handle::Handle( unsigned index, unsigned counter )
    : m_index( index )
    , m_counter( counter )
  {
  }

  Handle::Handle( unsigned handle )
  {
    *((unsigned *)this) = handle;
  }




}