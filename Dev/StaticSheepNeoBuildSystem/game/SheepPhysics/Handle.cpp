/*****************************************************************
Filename: Handle.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "precompiled.h"
#include "Handle.h"

namespace SheepFizz
{
  const Handle Handle::null;

  Handle::Handle( )
    : m_index( static_cast<size_t>(-1) )
    , m_counter( static_cast<size_t>(-1) )
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