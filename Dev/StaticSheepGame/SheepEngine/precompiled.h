#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else  // _DEBUG
#define DBG_NEW new
#endif

extern "C"
{
#include "lua/lua.h"
#include "lua/lua.hpp"
#include "lua/lauxlib.h"
#include "lua/luaconf.h"
#include "lua/lualib.h"
#include "lua/lfs.h"
}

// Engine API
#include "Api.h"

// Pre Reqs
#include <assert.h>

// Utilities
#include "Qualifiers.h" // Qualifier removal
#include "Macros.h" // Macro Helpers
#include "Utilities.h" // Debug macros and helpers

// Serialization
#include "File.h" // File handler
#include "Variable.h" // V
#include "Enum.h"
#include "Handle.h"
#include "TypeInfo.h"
#include "FuncSignature.h"
#include "Function.h"
#include "Hooks.h"
#include "LuaTransfer.h"
#include "LuaInterface.h"
#include "IntrospectionManager.h"

#include "Serialization.h"
#include "PODSerialization.h"
#include "ObjectAllocator.h"

#include "Generic.h"

#include "HandleManager.h"
#include "Component.h"
#include "ComponentCreator.h"
#include "Object.h"
#include "Archetype.h"
#include "SlotArray.h"
#include "Message.h"
#include "Space.h"
#include "System.h"
#include "GameLogic.h"
#include "Factory.h"
#include "Engine.h"


#include "CTester.h"
#include "CTransform.h"
#include "CLuaComponent.h"

#include "DefaultTypes.h"
#include "LuaDefaultBinds.h"





