/*****************************************************************
Filename: Space.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Space.h"
#include "Object.h"

namespace Framework
{

  Space::Space()
    : m_components({3,2}), 
    m_objects(sizeof(GameObject))
  {
    
  }

  HandleManager& Space::GetHandles()
  {
    return m_handles;
  }



}