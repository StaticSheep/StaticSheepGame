// EXTERNAL VERSION

#pragma once

#include "api.h"
#include <wtypes.h>

// These functions are exported and can be called from inside the engine (current project),
// and any other project which uses the dll
SHEEP_API int ExportedEngineFunction(void);
SHEEP_API int ExportedEnginePhysicsFunction(void);

SHEEP_API void TestStuff(HINSTANCE hInstance, int show);

class SHEEP_API Foo
{
public:
  void func1();
  void func2();
  int value1;
  
};

SHEEP_API Foo DoCoolStuff();



