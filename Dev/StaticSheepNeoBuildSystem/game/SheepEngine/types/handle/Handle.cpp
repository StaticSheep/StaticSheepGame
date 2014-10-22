/*****************************************************************
Filename: Handle.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "Handle.h"

namespace Framework
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


  void Handle::Serialize(File& file, Variable var)
  {
    Handle& h = var.GetValue<Handle>();
    GameSpace* space = (GameSpace*)Serializer::Get()->GetUserData();
    Generic* data = space->GetHandles().GetAs<Generic>(h);

    GameComponent* comp;
    const TypeInfo* type = GET_TYPE(int);

    if (data == nullptr)
      return;
    
    // From the generic type we can establish what sub type it is
    switch (data->genericType)
    {
    case eGameObject:
      // If it's an object, that's simple
      type = GET_TYPE(GameObject);
      break;
    case eGameComponent:
      // Components take a little more work
      comp = static_cast<GameComponent*>(data);
      type = GET_STR_TYPE(GET_ENUM(Component)->m_literals[comp->typeID].c_str());
      break;
    }

    // Establish a variable, and serialize
    Variable oVar(type, data);
    oVar.Serialize(file);
  }

}