/*****************************************************************
Filename: api.h
*****************************************************************/
#ifndef GRAPHICSAPI_H
#define GRAPHICSAPI_H


#ifdef DLL_GFXEXPORT
#define GFX_API __declspec(dllexport)
#else
#define GFX_API __declspec(dllimport)
#endif

#endif
