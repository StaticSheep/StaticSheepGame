/*****************************************************************
Filename: HandleManager.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include "Handle.h"

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

		void* Get(Handle handle) const;
		bool Exists(Handle handle) const;
		template <typename T>
		T* GetAs(Handle handle) const;

		unsigned FreeSlots(void) const;

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
		// Get the handle entry from the array
		const HandleEntry* entry = m_array + handle.m_index;

		if (entry->m_counter == handle.m_counter && entry->m_allocated == true)
		{
			return (T*)entry->m_ptr;
		}

		return nullptr;
	}
}



#endif