/*****************************************************************
Filename: api.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#ifndef GRAPHICSAPI_H
#define GRAPHICSAPI_H


#ifdef DLL_GFXEXPORT
#define GFX_API __declspec(dllexport)
#else
#define GFX_API __declspec(dllimport)
#endif

#endif