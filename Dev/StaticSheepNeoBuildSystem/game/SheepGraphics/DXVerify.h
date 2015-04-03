/******************************************************************************
Filename: DXVerify.h
Project:  GAM 250
Author(s): Scott Nelson (primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#if defined(_DEBUG)
#define DXVerify(exp) if(FAILED(exp)){const DWORD size = 100+1; WCHAR buffer[size]; \
					  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL, \
					  exp,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)buffer,size,NULL); \
					  ErrorIf(true, "sheep_grahics", "DX Error %s", buffer);}
#else
#ifndef DXVerify
#define DXVerify(x) (x)
#endif
#endif