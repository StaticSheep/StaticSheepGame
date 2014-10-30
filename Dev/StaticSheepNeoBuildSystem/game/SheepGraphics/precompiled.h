#include <assert.h>
#include <crtdbg.h>
#include <map>
#include <vector>
#include <string>

#include "Utilities.h"

#include "DirectXIncludes.h"
#include "WindowsIncludes.h"

#include "Vertices.h"
#include "DataTypes.h"



#if defined(_DEBUG)
#define DXVerify(exp) if(FAILED(exp)){const DWORD size = 100+1; WCHAR buffer[size]; \
					  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL, \
					  exp,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)buffer,size,NULL); \
					  ErrorIf(true, "sheep_grahics", "DX Error %s", buffer);}
#else
#define DXVerify(x) (x)
#endif