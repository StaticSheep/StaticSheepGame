
#include <stdio.h>

namespace Framework
{
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

    bool TraceLog(TRACELOG_LEVEL, const char* format, ...);

  private:

    void Format(TRACELOG_LEVEL);
    TRACELOG_LEVEL _level;
    FILE* file;

  };

  extern Trace* LOG;

}