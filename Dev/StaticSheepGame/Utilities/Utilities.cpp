/*****************************************************************
Filename: utilities.cpp
Project: 
Author(s): 

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Utilities.h"
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <consoleapi.h>
#include <io.h>
#include <fcntl.h>

size_t runtimeHash(const char *string, size_t len)
{
  unsigned int hash = 0;
  for(size_t i = 0; i < len; ++i)
  {
    hash = 65599 * hash + string[i];
  }
  return hash ^ (hash >> 16);
}

void OpenConsole()
{
  int outHandle, errHandle, inHandle;
  FILE *outFile, *errFile, *inFile;
  AllocConsole();
  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
  coninfo.dwSize.Y = 9999;
  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

  outHandle = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
  errHandle = _open_osfhandle((long)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
  inHandle = _open_osfhandle((long)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );

  outFile = _fdopen(outHandle, "w" );
  errFile = _fdopen(errHandle, "w");
  inFile =  _fdopen(inHandle, "r");

  //int a = SetConsoleMode((HANDLE)inHandle, ENABLE_ECHO_INPUT + ENABLE_LINE_INPUT);

  *stdout = *outFile;
  *stderr = *errFile;
  *stdin = *inFile;

  setvbuf( stdout, NULL, _IONBF, 0 );
  setvbuf( stderr, NULL, _IONBF, 0 );
  setvbuf( stdin, NULL, _IONBF, 0 );

  std::ios::sync_with_stdio();

}


DISABLE_WARNING(4996);
DISABLE_WARNING(4100);

void PrintError( const char *exp, const char *file, int line, const char *msg, ... )
{
  const int BufferSize = 2048;
  char formattedMsg[BufferSize];

  // Print out the file and line in visual studio format so the error can be
  // double clicked in the output window file(line) : error
  int offset = std::sprintf( formattedMsg, "%s(%d) : ", file, line );	
  if(msg != NULL)
  {
    va_list args;
    va_start(args, msg);
    vsnprintf( formattedMsg + offset, BufferSize, msg, args );
    vfprintf( stderr, msg, args );
    va_end(args);
  }
  else
  {
    std::strcpy( formattedMsg + offset, "No Error Message" );
    fprintf( stderr, formattedMsg );
  }

  fprintf( stderr, "\n" );
}

END_DISABLE();
END_DISABLE();