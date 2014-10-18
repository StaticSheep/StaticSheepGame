/*****************************************************************
Filename: File.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
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
    File();
    File(FILE* file);
    ~File();

    void Open(const char* name, FileAccess::Type accessLevel);
    void Close(void);

    void Write(const char* format, ...);
    void Read(const char* format, ...);

    void SeekFromBeginOfFile(int offset);
    void SeekFromEndOfFile(int offset);
    void SeekByOffset(int offset);

    std::string GetLine(char delim);  // Retrieve one line delimited by delim
    std::string GetLine(const char* delims);  // Retrieve one line delimited by chars within delims

    bool Validate(void) const;

    FILE* fp; // C file pointer
    char buffer[1024 * 32];

    static bool FileExists(const char* name);
  };


}