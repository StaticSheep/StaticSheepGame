#pragma once

#if defined(_DEBUG)
#include <DxErr.h>
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#if defined(_DEBUG)
#ifndef DXVerify
#define DXVerify(exp) if(FAILED(exp)){GFX_Release_D3D(); ErrorIf(true, "sheep_grahics", "DX Error %s", DXGetErrorDescription(exp));}
#endif
#else
#ifndef DXVerify
#define DXVerify(x) (x)
#endif
#endif

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <directxcolors.h>

