/*****************************************************************
Filename: HandleManager.h
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


	// Updates all handles to ensure that the HandleManager has accurate
  // pointers to the data which the handle needs to point at
}


/*
    // Re-construct the game object
*/



#endif
