
/*****************************************************************
Filename: SlotArray.h
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

		// Allocate an object with more than 1 argument



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

	template <typename T>



		// Destruct the data










}
