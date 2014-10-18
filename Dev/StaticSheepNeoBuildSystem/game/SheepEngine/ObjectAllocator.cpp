/*****************************************************************
Filename: ObjetAllocator.cpp
Project: 
Author(s): Zachary Nawar (Primary)

Thanks to Randy Gaul for the idea

This is essentially a vector class which has been modified to work
as a memory manager.

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "precompiled.h"
#include "ObjectAllocator.h"
#include <stdlib.h>
#include <string.h>

namespace Framework
{
  ObjectAllocator::ObjectAllocator()
    : m_array(nullptr), m_objectSize(1), m_size(0),
    m_capacity(0), m_grew(false)
  {

  }

	ObjectAllocator::ObjectAllocator(unsigned int objectSize)
		: m_array(nullptr), m_objectSize(objectSize), m_size(0),
		m_capacity(0), m_grew(false)
	{

	}

	ObjectAllocator::ObjectAllocator(unsigned int objectSize, unsigned int numElements)
		: m_array( malloc(objectSize * numElements) ), m_objectSize(objectSize), m_size(0),
		m_capacity(numElements), m_grew(false)
	{

  }

  void ObjectAllocator::Initialize(unsigned int objectSize, unsigned int numElements)
  {
    m_capacity = numElements;
    m_objectSize = objectSize;
    m_size = 0;

    free(m_array);

    m_array = malloc(objectSize * numElements);
  }

	void* ObjectAllocator::Allocate()
	{
		if (m_size == m_capacity)
			Grow();

		return (char*)m_array + m_size++ * m_objectSize;
	}

	void* ObjectAllocator::Free(void* data)
	{
		unsigned int index = GetIndex(data);

		// Rightmost now goes to the empty slot
		if (--m_size)
		{
			Move(index, m_size);
			return (char*)m_array + index * m_objectSize;
		}

		return nullptr;
	}

	void ObjectAllocator::Grow()
	{
		m_capacity = m_capacity * 3 + 10;

		m_grew = true;

		GrowInteral();
	}

	void ObjectAllocator::GrowInteral()
	{
		// Allocates a new array
		void* new_array = calloc(m_objectSize, m_capacity);

		// Copies over all of the data from the old array into the new
		memcpy(new_array, m_array, m_size * m_objectSize);

		// Free's the old array
		free(m_array);

		// Updates the pointers
		m_array = new_array;
	}

	void ObjectAllocator::Move(unsigned int a, unsigned int b)
	{
		void* A = (char*)m_array + a * m_objectSize;
		void* B = (char*)m_array + b * m_objectSize;
		memcpy(A, B, m_objectSize);
	}

	void ObjectAllocator::Shrink()
	{
		m_capacity = m_size;

		GrowInteral();
	}

	void* ObjectAllocator::operator[](unsigned int index)
	{
		assert( index < m_size );
		return (char*)m_array + index * m_objectSize;
	}

	const void* ObjectAllocator::operator[](unsigned int index) const
	{
		assert( index < m_size );
		return (char*)m_array + index * m_objectSize;
	}

	unsigned int ObjectAllocator::Size(void) const
	{
		return m_size;
	}

	unsigned int ObjectAllocator::Capacity(void) const
	{
		return m_capacity;
	}

	unsigned int ObjectAllocator::GetIndex( void* data ) const
	{
		return ((char*)data - (char*)m_array) / m_objectSize;
	}

	bool ObjectAllocator::Grew( void ) const
	{
		return m_grew;
	}

	void ObjectAllocator::ClearGrewFlag( void )
	{
		m_grew = false;
	}

  void ObjectAllocator::Clear()
  {
    m_capacity = 0;
    m_size = 0;
    m_objectSize = 0;

    if (m_array)
      free(m_array);
    
    m_array = nullptr;
  }

  ObjectAllocator::~ObjectAllocator()
  {
    Clear();
  }
}