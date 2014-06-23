/*****************************************************************
Filename: Space.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "ObjectAllocator.h"
#include "HandleManager.h"

namespace Framework
{
	class Space
	{
  public:
    Space();

    template <typename T>
    void SyncHandles(ObjectAllocator& m_allocator, bool force = false);



    ObjectAllocator* GetComponents(size_t type);
    //GameComponent* GetComponent(size_t type, Handle owner);

    HandleManager& GetHandles();
    
  private:
    // The collection of all game objects in this game space
    ObjectAllocator m_objects;

    // The collection of all components in this game space
    ObjectAllocator m_components[ecountComponents];

    // The handle manager for this game space
    HandleManager m_handles;
	};


  // Updates all handles to ensure that the HandleManager has accurate
  // pointers to the data which the handle needs to point at
  template <typename T>
  void Space::SyncHandles(ObjectAllocator& m_allocator, bool force)
  {
    if (m_allocator.Grew() || force)
    {
      // Keep in mind that allocators are essentially a giant vector of
      // every single component of a certain type. In the case in which
      // the allocator grows, the location of the data is now in a different
      // area of RAM and therefore we must update the handle manager
      // and tell it the new location of the components so the handles don't
      // point into memory that we don't own!
      for (auto i = m_allocator.begin<T>(); i != m_allocator.end<T>(); ++i)
      {
        m_handles.Update(&(*i), i->self);
      }

      m_allocator.ClearGrewFlag();
    }
  }
}