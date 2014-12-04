/*****************************************************************
Filename: precompiled.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#include <assert.h>
#include <crtdbg.h>
#include <map>
#include <vector>
#include <string>

#include "Utilities.h"

#include "DirectXIncludes.h"
#include "DataTypes.h"
#include "Texture/Tex2d.h"
#include "CBuffer.h"
#include "CBuffer_types.h"
#include "Effects\plight_effect.h"
#include "Effects\gen_effect.h"
#include "Camera\Camera.h"
#include "Handle.h"



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

#include "SafeRelease.h"