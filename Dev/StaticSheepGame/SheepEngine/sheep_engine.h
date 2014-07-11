// INTERNAL VERSION

#pragma once

#include "api.h"

// This function can be called from inside of the engine (current project)
// but cannot be called from another project since it is not exported
// !!! For our project however the header files we use for exporting functions
// SHOULD NEVER have function prototypes which are not exported !!!
int InternalEngineFunction(void);

// These functions are exported and can be called from inside the engine (current project),
// and any other project which uses the dll
SHEEP_API int ExportedEngineFunction(void);
SHEEP_API int ExportedEnginePhysicsFunction(void);
SHEEP_API int ExportedEngineGraphicsFunction(void);

SHEEP_API void TestStuff(void);

class SHEEP_API Foo
{
public:
  Foo() {};
  void func1();
  void func2();
  int value1;
};

SHEEP_API Foo DoCoolStuff();

