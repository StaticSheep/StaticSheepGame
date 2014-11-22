#pragma once

#define USE_EDITOR 0
#define USE_ANTTWEAKBAR 1
#define SHEEP_DEBUG 1

#pragma comment(lib, "SheepGraphics.lib")
#pragma comment(lib, "SheepPhysics.lib")
#pragma comment(lib, "SheepUtil.lib")

namespace Framework
{
  namespace Config
  {
    static int desiredWidth = 1600;
    static int desiredHeight = 900;
  }


  class Engine;

  Engine* AllocateEngine(void);

  void InitEngine(void);
}