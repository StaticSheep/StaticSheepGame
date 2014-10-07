/******************************************************************************
Filename: TraceLog.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include <stdio.h>

#pragma once

namespace Framework
{
  // for determining the levels of what to print
  enum TRACELOG_LEVEL
  {
    ERR = 0,
    WARNING,
    DEBUG,
    INFO
  };


  class Trace
  {
  public:

    Trace();
    Trace(TRACELOG_LEVEL, const char* = NULL);
    ~Trace();

    // method for printing out to the tracelog
    bool TraceLog(TRACELOG_LEVEL, const char* format, ...);

  private:
    // private method for printing a specific string
    void Format(TRACELOG_LEVEL);

    // keeps track of what levels for the tracelog to print
    TRACELOG_LEVEL _level;

    // the ouput file
    FILE* file;

  };

  // global pointer
  extern Trace* LOG;

}