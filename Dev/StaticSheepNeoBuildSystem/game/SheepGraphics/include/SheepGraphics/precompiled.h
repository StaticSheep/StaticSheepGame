#include <assert.h>
#include <crtdbg.h>
#include <map>
#include <vector>
#include <string>

#include "Mallocator.h"

#include "Utilities.h"

#include "dxerr.h"

#include "DirectXIncludes.h"
#include "WindowsIncludes.h"

#include "VertexTypes.h"
#include "DataTypes.h"



#if defined(_DEBUG)
#define DXVerify(exp) if(FAILED(exp)){ErrorIf(true, "sheep_grahics", "DX Error %s", DXGetErrorString(exp));}
#else
#define DXVerify(x) (x)
#endif
