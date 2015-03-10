/*****************************************************************
Filename: TraceLog.h
Project: 
Author(s): Zakary Wilson

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once
#include <stdio.h>

namespace Framework
{

  namespace TraceLevel
  {
    enum TRACELOG_LEVEL
    {
      ERR = 0,
      WARNING,
      DBG,
      INFO,
      VERBOSE
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

  // global pointer for the tracelog
  extern Tracelog* TRACELOG;

}