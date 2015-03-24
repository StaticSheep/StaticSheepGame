/*****************************************************************
Filename: Config.h
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#define USE_EDITOR 0
#define USE_ANTTWEAKBAR 1
#define SHEEP_DEBUG 1
#define SHEEPGRAPHICS 1
#define METRICS 0

#define PRESENTATION 0

#if METRICS
asldfkjsalfj
#endif

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

  Engine* AllocateEngine(int argc, char** argv);

  void InitEngine(void);
}