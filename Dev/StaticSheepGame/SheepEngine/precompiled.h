#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else  // _DEBUG
#define DBG_NEW new
#endif


#ifdef LUAJIT
extern "C"
{
#include "luajit/lua.h"
#include "luajit/lua.hpp"
#include "luajit/lauxlib.h"
#include "luajit/luaconf.h"
#include "luajit/lualib.h"
#include "luajit/lfs.h"
}
#else
extern "C"
{
#include "lua/lua.h"
#include "lua/lua.hpp"
#include "lua/lauxlib.h"
#include "lua/luaconf.h"
#include "lua/lualib.h"
#include "lua/lfs.h"
}
#endif





#include "api.h"
#include <assert.h>

#include "Qualifiers.h"
#include "Macros.h"
#include "Utilities.h"

#include "File.h"
#include "Variable.h"
#include "Enum.h"
#include "Handle.h"
#include "TypeInfo.h"
#include "FuncSignature.h"
#include "Function.h"
#include "Hooks.h"
#include "LuaTransfer.h"
#include "LuaInterface.h"
#include "IntrospectionManager.h"
#include "DefaultTypes.h"
#include "LuaDefaultBinds.h"

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
#include "SheepAudio.h"
#include "SheepGraphics.h"
#include "SheepPhysics.h"
#include "GameLogic.h"
#include "Factory.h"
#include "Engine.h"


#include "CTester.h"
#include "CTransform.h"
#include "CLuaComponent.h"





