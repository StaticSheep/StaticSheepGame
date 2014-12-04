/*****************************************************************
Filename: DirectXIncludes.h
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once


#include <Windows.h>
#include <Windowsx.h>

#if defined(_DEBUG)

#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <FW1FontWrapper.h>
#include <DirectXPackedVector.h>
#include "SpriteBatch.h"


