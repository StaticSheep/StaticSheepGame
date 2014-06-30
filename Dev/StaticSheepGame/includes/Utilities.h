/*****************************************************************
Filename: utilities.h
Project: 
Author(s): 

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#define STRINGIZE( _ ) #_

#define H1(s,i,x)   (x*65599u+(uint8_t)s[(i)<strlen(s)?strlen(s)-1-(i):strlen(s)])
#define H4(s,i,x)   H1(s,i,H1(s,i+1,H1(s,i+2,H1(s,i+3,x))))
#define H16(s,i,x)  H4(s,i,H4(s,i+4,H4(s,i+8,H4(s,i+12,x))))
#define H64(s,i,x)  H16(s,i,H16(s,i+16,H16(s,i+32,H16(s,i+48,x))))
#define H256(s,i,x) H64(s,i,H64(s,i+64,H64(s,i+128,H64(s,i+192,x))))

#define Hash(s)    ((uint32_t)(H256(s,0,0)^(H256(s,0,0)>>16)))
#define GenerateHash(str) runtimeHash(str.c_str(), str.length())

size_t runtimeHash(const char *string, size_t len);

void OpenConsole();

#define TODO( MESSAGE ) \
  __pragma( message (__FILE__ "(" STRINGIZE(__LINE__) ") : TODO [ " MESSAGE " ]") )

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


#define Error(MODULE, FORMAT, ...) \
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

