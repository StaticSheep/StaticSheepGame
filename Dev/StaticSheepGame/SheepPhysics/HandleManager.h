/*****************************************************************
Filename: HandleManager.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include "Handle.h"
#include "ObjectAllocator.h"

namespace SheepFizz
{
	class HandleManager
	{
	public:
		HandleManager();

		void Clear(void);
		Handle Insert(void* ptr);
		void Replace(void* ptr, Handle& handle);
		void Update(void* ptr, Handle& handle);
		void Remove(Handle handle);

		void* Get(Handle handle) const;
		bool Exists(Handle handle) const;
		template <typename T>
		T* GetAs(Handle handle) const;

		unsigned FreeSlots(void) const;

		static const int m_MaxEntries = 16384; // 2^14

		template <typename T>
		void SyncHandles(ObjectAllocator& m_allocator, bool force = false);

	private:
		struct HandleEntry
		{
			HandleEntry();
			HandleEntry(unsigned nextFreeIndex);

			unsigned m_nextFreeIndex : 16;
			unsigned m_counter : 16;
			unsigned m_endOfList : 1;
			unsigned m_allocated : 1;
			void* m_ptr;
		};

		unsigned m_freeSlots;
		unsigned m_freeList;
		HandleEntry m_array[m_MaxEntries];
	};

	template <typename T>
	T* HandleManager::GetAs(Handle handle) const
	{
		// Get the handle entry from the array
		const HandleEntry* entry = m_array + handle.m_index;

		if (entry->m_counter == handle.m_counter && entry->m_allocated == true)
		{
			return (T*)entry->m_ptr;
		}

		return nullptr;
	}

	// Updates all handles to ensure that the HandleManager has accurate
  // pointers to the data which the handle needs to point at
  template <typename T>
  void HandleManager::SyncHandles(ObjectAllocator& m_allocator, bool force)
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
        Update(&(*i), i->self);
      }

      m_allocator.ClearGrewFlag();
    }
  }
}


/*

  GameObject* GameSpace::CreateEmptyObject()
  {
    // Allocate space for a new object
    GameObject* object = (GameObject*)m_objects.Allocate();

    // Re-construct the game object
    new (object) GameObject();

    // Register the object inside of the handle manager
    object->self = GetHandles().Insert(object);
    object->space = this;
    object->guid = m_guid++;
    object->m_active = true;

    // Check to see if we need to sync the GameObject handles
    GetHandles().SyncHandles<GameObject>(m_objects);

    return object;
  }

  GameObject* GameSpace::GetGameObject(Handle handle)
  {
    return m_handles.GetAs<GameObject>(handle);
  }

  void GameSpace::RemoveGameObject(GameObject* object)
  {
    //std::cout << "Removing game object: " << object->self.operator size_t() << "\n";

    // Removes all components
    object->~GameObject();

    Lua::CallFunc(ENGINE->Lua(), "RemoveGameObject", m_name, object->self.operator size_t());

    // Remove the object from the handle manager
    GetHandles().Remove(object->self);

    // Remove the object from the object list
    GameObject* moved = (GameObject*)m_objects.Free(object);

    // When we freed the object the right most element
    // was moved to take the old objects place so now
    // we have to update the handle manager and tell it
    // to point at the right place in memory
    if (moved)
      GetHandles().Update(moved, moved->self);
  }
*/



#endif