#include "precompiled.h"
#include "Handle.h"

namespace DirectSheep
{
  Handle::Handle() : index(0), type(NONE) {}

  ObjectType Handle::GetType(void) const
  {
    return type;
  }

  int Handle::GetIndex(void) const
  {
    return index;
  }

  bool Handle::operator==(const Handle& rhs) const
  {
    if(index == rhs.index && type == rhs.type)
      return true;
    else
      return false;
  }

  bool Handle::operator!=(const Handle& rhs) const
  {
    if(index != rhs.index && type != rhs.type)
      return true;
    else
      return false;
  }

  bool Handle::operator<(const Handle& rhs) const
  {
    if(index < rhs.index)
      return true;
    else
      return false;
  }

  Handle::Handle(ObjectType type, int index) : type(type), index(index) {}
  Handle::Handle(ObjectType type, void* ptr) : type(type), ptr(ptr) {}


}