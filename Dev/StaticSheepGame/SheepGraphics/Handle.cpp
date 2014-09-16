#include "Handle.h"
namespace DirectSheep
{
  Handle::Handle() : index(0), type(NONE) {}

  ObjectType Handle::GetType(void) const
  {
    return type;
  }

  int Handle::operator*(void) const
  {

  }

  Handle::operator bool(void) const
  {

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


}