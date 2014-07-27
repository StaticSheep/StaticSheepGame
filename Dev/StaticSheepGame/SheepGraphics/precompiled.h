#include <assert.h>
#include <crtdbg.h>

#include "api.h"

#include "Utilities.h"


#include "WindowsIncludes.h"
#include "DirectXIncludes.h"
#include "VertexTypes.h"
#include "GFX_Core.h"

#include "sheep_graphics.h"




#if defined(_DEBUG)
#define DXVerify(exp) if(FAILED(exp)){DirectSheep::GFX_Release_D3D(); ErrorIf(true, "sheep_grahics", "DX Error %s", DXGetErrorDescription(exp));}
#else
#define DXVerify(x) (x)
#endif