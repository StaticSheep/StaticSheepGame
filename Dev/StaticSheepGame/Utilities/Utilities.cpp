/*****************************************************************
Filename: utilities.cpp
Project: 
Author(s): 

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Utilities.h"

size_t runtimeHash(const char *string, size_t len)
{
  unsigned int hash = 0;
  for(size_t i = 0; i < len; ++i)
  {
    hash = 65599 * hash + string[i];
  }
  return hash ^ (hash >> 16);
}
