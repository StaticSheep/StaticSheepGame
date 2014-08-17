
/*****************************************************************
Filename: SlotArray.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
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
		SlotArray();

		SlotArray(unsigned int numElements);

    ~SlotArray();

		// Allocation
		unsigned Allocate(void);

		// Allocate an object with a conversion constructor
		template <typename Arg1>
		unsigned Allocate1(const Arg1& arg1);

		// Allocate an object with more than 1 argument
		template <typename Arg1, typename Arg2>
		unsigned Allocate2(const Arg1 arg1, const Arg2 arg2);

		template <typename Arg1, typename Arg2, typename Arg3>
		unsigned Allocate3(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3);

		template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		unsigned Allocate4(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4);

		// Insert / Copy
		unsigned int Push_back(T* obj);

		// Iterators
		//inline typename std::iterator<T> begin(void) {return ArrayIterator<T>(m_array);}
		//inline typename std::iterator<T> begin(void) {return ArrayIterator<T>(m_array + m_size);}

		// Deleting
		void Free(unsigned int index);
		// Free's an element from inside of the array.
		// Takes a pointer to the element to free
		T* Free(T *val);

		// Swap memory of two indices
		//void Swap(unsigned int a, unsigned int b);

		// Subscript operators
		T* operator[](unsigned int index);
		const T* operator[](unsigned int index) const;

		// Helpers
		unsigned int Size(void) const;
		unsigned int Capacity(void) const;
		unsigned int GetIndex(T* data) const;
		bool Grew(void) const;
		void ClearGrewFlag(void);

	private:
		T* m_array;

		unsigned int m_size;
		unsigned int m_capacity;
		bool m_grew;

		// Grow the array (INTERNAL)
		void Grow(void);
		void GrowInternal(void);

    void Clear();

		// Move data from one index to another (Like swapping)
		void Move(unsigned int a, unsigned int b);
	};

	template <typename T>
	SlotArray<T>::SlotArray() : m_array(NULL), m_size(0), m_capacity(0), m_grew(false) {};
	
	template <typename T>
	SlotArray<T>::SlotArray(unsigned int numElements) : m_array( (T*)malloc(sizeof(T) * numElements) ), m_size(0), m_capacity(numElements), m_grew(false) {};

  template <typename T>
  SlotArray<T>::~SlotArray()
  {
    Clear();
  }

  template <typename T>
  void SlotArray<T>::Clear()
  {
    if (m_array)
      delete m_array;
  }


	template <typename T>
	unsigned int SlotArray<T>::Allocate( void )
	{
		if(m_size == m_capacity)
			Grow( );

		T *val = m_array + m_size;

		// This creates a new object via the default constructor
		new (val) T( );
		return m_size++;
	}

	template <typename T>
	template <typename Arg1>
	unsigned int SlotArray<T>::Allocate1( const Arg1& arg1 )
	{
		if (m_size == m_capacity)
			Grow();

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff
		new (val) T(std::forward<Arg1>(arg1));

		return m_size++;
	}

	template <typename T>
	template <typename Arg1, typename Arg2>
	unsigned int SlotArray<T>::Allocate2(const Arg1 arg1, const Arg2 arg2)
	{
		if (m_size == m_capacity)
			Grow();

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff
		new (val) T(arg1, arg2);

		return m_size++;
	}

	template <typename T>
	template <typename Arg1, typename Arg2, typename Arg3>
	unsigned int SlotArray<T>::Allocate3(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3)
	{
		if(m_size == m_capacity)
			Grow( );

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff
		new (val) T(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3));
		return m_size++;
	}

	template <typename T>
	template <typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	unsigned int SlotArray<T>::Allocate4(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4)
	{
		if(m_size == m_capacity)
			Grow( );

		T *val = m_array + m_size;

		// This nifty guy does nifty stuff
		new (val) T(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3), std::forward<Arg4>(arg4));
		return m_size++;
	}

	template <typename T>
	unsigned int SlotArray<T>::Push_back(T* obj)
	{
		if(m_size == m_capacity)
			Grow( );

		T *val = m_array + m_size;

		*val = *obj;

		return m_size++;
	}

	template <typename T>
	void SlotArray<T>::Free(unsigned int index)
	{
		// Call the destructor on the index
		T* val = (T*)m_array + index;
		val->~T();

		// Move the right most element to take it's place
		Move(index, m_size - 1);
		--m_size;
	}

	template <typename T>
	T *SlotArray<T>::Free(T* val)
	{
		unsigned int index = val - m_array;

		// Destruct the data
		val->~T();

		// Move the right most element to take it's place
		Move(index, m_size - 1);

		--m_size;

		return m_array + index;
	}

	template <typename T>
	void SlotArray<T>::Grow(void)
	{
		// Double the capacity by 1.5x and + 10
		m_capacity = (int)(m_capacity * 1.5f + 10);

		// Set the flag
		m_grew = true;

		GrowInternal();
	}

	template <typename T>
	void SlotArray<T>::GrowInternal()
	{
		// Allocate a new array
		T *array = (T*)calloc(m_capacity, sizeof(T));

		// Copy over the elements
		for (unsigned i = 0; i < m_size; ++i)
			new (array + i) T(m_array[i]);

		// Free the old data
		free(m_array);

		// Fix our pointer
		m_array = array;
	}

	template <typename T>
	void SlotArray<T>::Move(unsigned int a, unsigned int b)
	{
		m_array[a] = std::move(m_array[b]);
	}

	template <typename T>
	T* SlotArray<T>::operator[](unsigned int index)
	{
		return (T*)m_array + index;
	}

	template <typename T>
	const T* SlotArray<T>::operator[](unsigned int index) const
	{
		return (T*)m_array + index;
	}

	template <typename T>
	unsigned int SlotArray<T>::Size() const
	{
		return m_size;
	}

	template <typename T>
	unsigned int SlotArray<T>::Capacity() const
	{
		return m_capacity;
	}

	template <typename T>
	unsigned int SlotArray<T>::GetIndex(T* data) const
	{
		return data - m_array;
	}

	template <typename T>
	bool SlotArray<T>::Grew() const
	{
		return m_grew;
	}

	template <typename T>
	void SlotArray<T>::ClearGrewFlag()
	{
		m_grew = false;
	}
}