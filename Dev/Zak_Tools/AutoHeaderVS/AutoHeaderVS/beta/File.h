/*****************************************************************
Filename: File.h
*****************************************************************/

#pragma once
#include <mbstring.h>
#include <string>

namespace Framework
{
  namespace FileAccess
  {
    enum Type
    {
      Read,
      Write,
      Append
    };
  };

  struct File
  {
  public:






    FILE* fp; // C file pointer
    char buffer[1024 * 32];

  };


}
