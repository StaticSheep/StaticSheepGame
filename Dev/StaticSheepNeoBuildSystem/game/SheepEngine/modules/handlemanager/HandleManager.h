/*****************************************************************
Filename: HandleManager.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include "types/handle/Handle.h"

namespace Framework
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

    void Run(void(*runFunc)(void* ptr));

		void* Get(Handle handle) const;
		bool Exists(Handle handle) const;
		template <typename T>
		T* GetAs(Handle handle) const;

		unsigned FreeSlots(void) const;

    template<typename T>
    void SyncHandles(ObjectAllocator& allocator, bool force = false);

		static const int m_MaxEntries = 16384; // 2^14

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
    if (handle == Handle::null)
      return nullptr;

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



#endif