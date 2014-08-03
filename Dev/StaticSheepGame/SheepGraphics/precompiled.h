#include <assert.h>
#include <crtdbg.h>
#include <map>
#include <vector>
#include <string>

#include "api.h"

#include "Utilities.h"


#include "WindowsIncludes.h"
#include "DirectXIncludes.h"
#include "VertexTypes.h"
#include "GFX_Core.h"
#include "Camera.h"

#include "sheep_graphics.h"




#if defined(_DEBUG)
#define DXVerify(exp) if(FAILED(exp)){DirectSheep::ReleaseD3D(); ErrorIf(true, "sheep_grahics", "DX Error %s", DXGetErrorDescription(exp));}
#else
#define DXVerify(x) (x)
#endif