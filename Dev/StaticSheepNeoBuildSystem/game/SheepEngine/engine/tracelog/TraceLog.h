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

  namespace TraceLevel
  {
    enum TRACELOG_LEVEL
    {
      ERR = 0,
      WARNING,
      DEBUG,
      INFO
    };
  }
  


  class Tracelog
  {
  public:

    Tracelog();
    Tracelog(TraceLevel::TRACELOG_LEVEL, const char* = NULL);
    ~Tracelog();

    void Initialize(TraceLevel::TRACELOG_LEVEL, const char* = NULL);

    // method for printing out to the tracelog
    bool Log(TraceLevel::TRACELOG_LEVEL, const char* format, ...);

  private:
    // private method for printing a specific string
    void Format(TraceLevel::TRACELOG_LEVEL);

    // keeps track of what levels for the tracelog to print
    TraceLevel::TRACELOG_LEVEL _level;

    // the ouput file
    FILE* file;

  };

  // global pointer
  extern Tracelog* TRACELOG;

}