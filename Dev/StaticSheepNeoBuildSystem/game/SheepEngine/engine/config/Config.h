#pragma once

#define USE_EDITOR 1
#define USE_ANTTWEAKBAR 1

namespace Framework
{
  class Engine;

  Engine* AllocateEngine(void);

  void InitEngine(void);
}