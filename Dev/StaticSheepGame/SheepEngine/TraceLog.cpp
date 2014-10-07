#include "TraceLog.h"
#include <time.h>

namespace Framework
{
  Trace* LOG = NULL;

  Trace::Trace()
  {
    file = NULL;
    LOG = this;
  }

  Trace::Trace(TRACELOG_LEVEL level, const char* filename)
  {
    LOG = this;
    _level = level;
    if(filename != NULL)
      file = fopen(filename, "wt");
    else
      file = NULL;
  }

  Trace::~Trace()
  {
    if(file != NULL)
      fclose(file);
  }

  bool Trace::TraceLog(TRACELOG_LEVEL level, const char* format, ...)
  {
    if(level > _level)
      return false;

    int check;
    va_list args;
    va_start (args, format);

    Format(level);

    if(file != NULL)
    {
      check = vfprintf (file, format, args);
    }
    else
    {
      check = vprintf (format, args);
    }

    va_end (args);

    if(check > 0)
      return true;
    else
      return false;
  }

  void Trace::Format(TRACELOG_LEVEL level)
  {
    time_t currentTime;
    time(&currentTime);

    struct tm* dateTime;

    dateTime = localtime(&currentTime);
    char* theTime = asctime(dateTime);
    char* reader = theTime;
    char* begin;
    char* last;
    int count = 0;

    while(reader++)
    {
      if(*reader == '\n')
      {
        *reader = '\0';
        break;
      }

      if(*reader == ' ')
      {
        ++count;
        last = reader;
        if(count == 3)
          begin = reader + 1;
      }
    }

    *last = '\0';

    if(file != NULL)
    {
      switch(level)
      {
      case ERR:
        fprintf(file, "%s [ERROR]    ", begin);
        break;
      case WARNING:
        fprintf(file, "%s [WARNING]  ", begin);
        break;
      case DEBUG:
        fprintf(file, "%s [DEBUG]    ", begin);
        break;
      case INFO:
        fprintf(file, "%s [INFO]     ", begin);
        break;
      default:
        break;
      }
    }
    else
    {
      switch(level)
      {
      case ERR:
        printf("%s [ERROR]    ", begin);
        break;
      case WARNING:
        printf("%s [WARNING]  ", begin);
        break;
      case DEBUG:
        printf("%s [DEBUG]    ", begin);
        break;
      case INFO:
        printf("%s [INFO]     ", begin);
        break;
      default:
        break;
      }
    }
  }

}