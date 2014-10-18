/*****************************************************************
Filename: utilities.h
Project: 
Author(s): 

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

void OpenConsole();


void PrintError( const char *exp, const char *file, int line, const char *msg, ... );

#define DEBUG_BREAK( ) \
  __debugbreak( )


// Don't forget to use END_DISABLE
#define DISABLE_WARNING( WARN_ID ) \
  __pragma( warning( push ) ) \
  __pragma( warning( disable : WARN_ID ) )

// Disables a warning without the push call
#define PERM_DSIABLE_WARNING( WARN_ID ) \
  __pragma( warning( disable : WARN_ID ) )

#define END_DISABLE( ) \
  __pragma( warning( pop ) )


#ifdef _DEBUG

#define FORCEERROR(MODULE, FORMAT, ...) \
  DISABLE_WARNING(4127); \
  do {\
  PrintError( "Forced Error", __FILE__, __LINE__, FORMAT, __VA_ARGS__ ); \
  _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, MODULE, "Forced Error\n"##FORMAT, __VA_ARGS__); \
  _CrtDbgBreak(); \
  } while(0) \
  END_DISABLE();


#define ErrorIf( EXP, MODULE, FORMAT, ... )                                \
  DISABLE_WARNING(4127); \
  do                                                       \
{                                                        \
  if(EXP)                                                \
{                                                      \
  PrintError( #EXP, __FILE__, __LINE__, FORMAT, __VA_ARGS__ ); \
  _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, MODULE, #EXP##"\n"##FORMAT, __VA_ARGS__); \
  _CrtDbgBreak(); \
}                                                      \
} while(0) \
END_DISABLE();

#else
#define Error(...)
#define ErrorIf(...)
#endif
