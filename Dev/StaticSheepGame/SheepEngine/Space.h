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

    // The handle manager for the space, I kind of just want this to be public
    HandleManager m_handles;
  private:

	};


  // Updates all handles to ensure that the HandleManager has accurate
  // pointers to the data which the handle needs to point at
  template <typename T>
  void Space::SyncHandles(ObjectAllocator& m_allocator, bool force)
  {
    if (m_allocator.Grew() || force)
    {
      for (auto i = m_allocator.begin<T>(); i != m_allocator.end<T>(); ++i)
      {
        m_handles.Update(&(*i), i->self);
      }

      m_allocator.ClearGrewFlag();
    }
  }
}