/*****************************************************************
Filename: api.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once


#ifdef DLL_PHYEXPORT
#define PHY_API __declspec(dllexport)
#else
#define PHY_API __declspec(dllimport)
#endif