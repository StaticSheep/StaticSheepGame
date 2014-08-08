
/*****************************************************************
Filename: SlotArray.h
Project: 

*****************************************************************/

#pragma once

#include <type_traits>

// A slot array is essentially the same thing as a plain old vector
// With the exception that when an element is deleted, the right-most
// element replaces the removed element.
// Thanks to Randy Gaul for the idea

namespace Framework
{
	template <typename T>
	class SlotArray
	{
	public:



		// Allocation

		// Allocate an object with a conversion constructor
		template <typename Arg1>

		// Allocate an object with more than 1 argument
		template <typename Arg1, typename Arg2>

		template <typename Arg1, typename Arg2, typename Arg3>

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>

		// Insert / Copy

		// Iterators

		// Deleting
		// Free's an element from inside of the array.
		// Takes a pointer to the element to free

		// Swap memory of two indices

		// Subscript operators

		// Helpers

	private:
		T* m_array;

		unsigned int m_size;
		unsigned int m_capacity;
		bool m_grew;



	};

	template <typename T>
	
	template <typename T>

  template <typename T>
  {
  }

  template <typename T>
  {
      delete m_array;
  }


	template <typename T>
	{

		T *val = m_array + m_size;

		// This creates a new object via the default constructor
		return m_size++;
	}

	template <typename T>
	template <typename Arg1>
	{

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff

		return m_size++;
	}

	template <typename T>
	template <typename Arg1, typename Arg2>
	{

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff

		return m_size++;
	}

	template <typename T>
	template <typename Arg1, typename Arg2, typename Arg3>
	{

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff
		return m_size++;
	}

	template <typename T>
	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	{

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff
		return m_size++;
	}

	template <typename T>
	{

		T *val = m_array + m_size;

		*val = *obj;

		return m_size++;
	}

	template <typename T>
	{
		// Call the destructor on the index

		// Move the right most element to take it's place
		--m_size;
	}

	template <typename T>
	{
		unsigned int index = val - m_array;

		// Destruct the data

		// Move the right most element to take it's place

		--m_size;

		return m_array + index;
	}

	template <typename T>
	{
		// Double the capacity by 1.5x and + 10

		// Set the flag
		m_grew = true;

	}

	template <typename T>
	{
		// Allocate a new array

		// Copy over the elements

		// Free the old data

		// Fix our pointer
		m_array = array;
	}

	template <typename T>
	{
	}

	template <typename T>
	{
	}

	template <typename T>
	{
	}

	template <typename T>
	{
		return m_size;
	}

	template <typename T>
	{
		return m_capacity;
	}

	template <typename T>
	{
		return data - m_array;
	}

	template <typename T>
	{
		return m_grew;
	}

	template <typename T>
	{
		m_grew = false;
	}
}
