/*****************************************************************
Filename: ObjectAllocator.h
Project: 

Thanks to Randy Gaul for the idea

This is essentially a vector class which has been modified to work
as a memory manager.

*****************************************************************/

#pragma once
#include <xutility>

namespace Framework
{
	class ObjectAllocator
	{
	public:

		// Non-Default allocator

		// Pre-allocates space


		// Allocate a single object, returns a pointer to the object, does not call new

		template <typename T>
		template <typename T>

		// Move rightmost element into deleted slot, does not placement delete


		// Shrink internal array to exactly number of elements



	private:
		void* m_array; // Pointer to the array

		unsigned m_objectSize; // The size of individual objects
		unsigned m_size; // How many elements there are in the array
		unsigned m_capacity; // The capacity of the array
		bool m_grew; // If the array was grown

		// Grow internal array

		// Move data from one index to another
	};
}
