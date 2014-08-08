/*****************************************************************
Filename: HandleManager.h
Project: 

*****************************************************************/

#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include "Handle.h"

namespace Framework
{
	class HandleManager
	{
	public:


		template <typename T>


		static const int m_MaxEntries = 16384; // 2^14

	private:
		struct HandleEntry
		{

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
	{
		// Get the handle entry from the array
		const HandleEntry* entry = m_array + handle.m_index;

		{
		}

		return nullptr;
	}
}



#endif
