/*****************************************************************
Filename: api.h
Project: 

*****************************************************************/

#pragma once

#ifdef DLL_ENGINEEXPORT
// If this was included from inside of the engine project, then SHEEP_API means EXPORT
#define SHEEP_API __declspec(dllexport)
#else
// If this was included from another project, then SHEEP_API means IMPORT
#define SHEEP_API __declspec(dllimport)
#endif

