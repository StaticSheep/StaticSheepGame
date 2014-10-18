#pragma once

#include "precompiled.h"

// The following headers are required for all allocators.
#include <stddef.h>  // Required for size_t and ptrdiff_t and NULL
#include <new>       // Required for placement new and std::bad_alloc
#include <stdexcept> // Required for std::length_error

// The following headers contain stuff that Mallocator uses.
#include <stdlib.h>  // For malloc() and free()
#include <iostream>  // For std::cout
#include <ostream>   // For std::endl
#include <list>      // For std::list

template <typename T> class Mallocator {
public:

  // The following will be the same for virtually all allocators.
  typedef T * pointer;
  typedef const T * const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T value_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
    return &r;
  }
    return &s;
  }
    // The following has been carefully written to be independent of
    // the definition of size_t and to avoid signed/unsigned warnings.
  }


  // The following must be the same for all allocators.
  template <typename U> struct rebind {
    typedef Mallocator<U> other;
  };
  }

  }



  // Returns true if and only if storage allocated from *this
  // can be deallocated from other, and vice versa.
    return true;
  }


  // Default constructor, copy constructor, rebinding constructor, and destructor.


    // in which case the bad_alloc check below would fire).
      return NULL;
    }

    // All allocators should contain an integer overflow check.
    // The Standardization Committee recommends that std::length_error
    }

    }

  }
    // Mallocator prints a diagnostic message to demonstrate
  }


  }


  // Allocators are not required to be assignable, so
  // all allocators should have a private unimplemented
  // "assignment operator could not be generated because a
  // base class assignment operator is inaccessible" within
  // the STL headers, but that warning is useless.
private:
};

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
