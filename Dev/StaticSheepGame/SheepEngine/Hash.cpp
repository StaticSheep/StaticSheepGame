/*****************************************************************
Filename: Hash.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

unsigned int generateHash(const char *string, size_t len)
{
  unsigned int hash = 0;
  for(size_t i = 0; i < len; ++i)
  {
    hash = 65599 * hash + string[i];
  }
  return hash ^ (hash >> 16);
}
