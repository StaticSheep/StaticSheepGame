/*****************************************************************
Filename: Debug.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/Precompiled.h"
#include "TraceLog.h"
#include <time.h>

namespace Framework
{
  Tracelog* TRACELOG = NULL;

  using namespace TraceLevel;

  Tracelog::Tracelog()
  {
    file = NULL;
    TRACELOG = this;
    _level = DBG;

    Log(INFO, "TraceLog Started\n");
  }

  // Set the level of the TraceLog and open output file if given.
  Tracelog::Tracelog(TRACELOG_LEVEL level, const char* filename)
  {
    TRACELOG = this;
    _level = level;
    if(filename != NULL)
      file = fopen(filename, "wt");
    else
      file = NULL;

    Log(INFO, "TraceLog Started\n");
  }

  void Tracelog::Initialize(TRACELOG_LEVEL level, const char* filename)
  {
    TRACELOG = this;
    _level = level;
    if (filename != NULL)
      file = fopen(filename, "wt");
    else
      file = NULL;

    Log(INFO, "TraceLog Initialized\n");
  }

  Tracelog::~Tracelog()
  {
    Log(INFO, "TraceLog Ending\n");

    if(file != NULL)
      fclose(file);
  }

  // Set the level if severity of the message, then use like printf
  bool Tracelog::Log(TRACELOG_LEVEL level, const char* format, ...)
  {
    // if the level is too high, don't do anything
    if(level > _level)
      return false;

    // otherwise we are going to do some things
    int check;

    // declare a variadic list of arguments...
    va_list args;

    // initialize it with what was passed in
    va_start (args, format);

    // do some formatting and printing. Puts the current time and the type
    // of level of the string
    Format(level);

    // check to see if we have a file
    if(file != NULL)
    {
      // in which case, print into it
      check = vfprintf(file, format, args);
      fprintf(file, "\n");
    }
    else // otherwise
    {
      // print to the console
      check = vprintf(format, args);
      printf("\n");
    }

    // remove the args
    va_end (args);

    // if something was printed correctly
    if(check > 0)
      return true; // return true...
    else
      return false;
  }

  void Tracelog::Format(TRACELOG_LEVEL level)
  {
    // get the current time in seconds since 1970
    time_t currentTime;
    time(&currentTime);

    struct tm* dateTime;

    // convert it into local time
    dateTime = localtime(&currentTime);

    // convert that into ascii
    char* theTime = asctime(dateTime);

    // then get some char* to alter the output
    char* reader = theTime;
    char* begin;
    char* last;

    // keeps track of the amount of spaces
    int count = 0;

    // iterate through theTime 
    while(reader++)
    {
      // we are looking for the \n because we want to get rid of it
      if(*reader == '\n')
      {
        *reader = '\0';
        break;
      }

      // also if we find a space...
      if(*reader == ' ')
      {
        ++count;

        // keep track of the last space
        last = reader;

        // to get rid of the month and day
        if(count == 3)
          begin = reader + 1;
      }
    }
    // to get rid of the year
    *last = '\0';

    // if we have a file...
    if(file != NULL)
    {
      // print specific things depending on the level
      switch(level)
      {
      case ERR:
        fprintf(file, "%s [ERROR]    ", begin);
        break;
      case WARNING:
        fprintf(file, "%s [WARNING]  ", begin);
        break;
      case DBG:
        fprintf(file, "%s [DEBUG]    ", begin);
        break;
      case INFO:
        fprintf(file, "%s [INFO]     ", begin);
        break;
      default:
        break;
      }
    }
    else // else, do it to the console
    {
      switch(level)
      {
      case ERR:
        printf("%s [ERROR]    ", begin);
        break;
      case WARNING:
        printf("%s [WARNING]  ", begin);
        break;
      case DBG:
        printf("%s [DEBUG]    ", begin);
        break;
      case INFO:
        printf("%s [INFO]     ", begin);
        break;
      default:
        break;
      }
    }
  }// end function

}// end namespace
