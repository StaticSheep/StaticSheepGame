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
#include "DataTypes.h"
#include "Camera.h"

#include "System.h"
#include "RenderMain.h"




#if defined(_DEBUG)
#define DXVerify(exp) if(FAILED(exp)){DirectSheep::Release(); ErrorIf(true, "sheep_grahics", "DX Error %s", DXGetErrorDescription(exp));}
#else
#define DXVerify(x) (x)
#endif