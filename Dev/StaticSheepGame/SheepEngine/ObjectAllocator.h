/*****************************************************************
Filename: ObjectAllocator.h
Project: 
Author(s): Zachary Nawar (Primary)

Thanks to Randy Gaul for the idea

This is essentially a vector class which has been modified to work
as a memory manager.

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include <xutility>

namespace Framework
{
	class ObjectAllocator
	{
	public:
    ObjectAllocator();

		// Non-Default allocator
		ObjectAllocator(unsigned int objectSize);

		// Pre-allocates space
		ObjectAllocator(unsigned int objectSize, unsigned int numElements);

    ~ObjectAllocator();

		// Allocate a single object, returns a pointer to the object, does not call new
		void* Allocate();

		template <typename T>
		inline T* begin(void) { return (T*)m_array; }
		template <typename T>
		inline T* end(void) { return (T*)m_array + m_size; }

		// Move rightmost element into deleted slot, does not placement delete
		void* Free(void* memory); // Returns pointer to the moved element (was rightmost element before free)

    void Clear();

		// Shrink internal array to exactly number of elements
		void Shrink(void);

		void* operator[](unsigned index);
		const void* operator[](unsigned index) const;
		unsigned Size(void) const; // Number of allocated elements
		unsigned Capacity(void) const; // Number of elements total
		unsigned GetIndex(void* data) const;
		bool Grew(void) const;
		void ClearGrewFlag(void);

    void Initialize(unsigned int objectSize, unsigned int numElements);

	private:
		void* m_array; // Pointer to the array

		unsigned m_objectSize; // The size of individual objects
		unsigned m_size; // How many elements there are in the array
		unsigned m_capacity; // The capacity of the array
		bool m_grew; // If the array was grown

		// Grow internal array
		void Grow();
		void GrowInteral();

		// Move data from one index to another
		void Move(unsigned int a, unsigned int b);
	};
}