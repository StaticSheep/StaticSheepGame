#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
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


#include "File.h" // File handler
#include "Variable.h" // Variable delegate
#include "Enum.h" // Enumeration Serialization/Introspection
#include "TypeInfo.h" // Type introspection
#include "Serialization.h" // Serialization
#include "PODSerialization.h" // Plain old Data serialization routines

#include "Function.h" // Function binding

#include "IntrospectionManager.h" // Introspection

#include "LuaTransfer.h" // Lua<->C++ Transfer
#include "LuaInterface.h" // Lua interface

#include "Handle.h" // Handle type

#include "ObjectAllocator.h" // Object Memory manager
#include "Generic.h" // Generic Object/Component type

#include "HandleManager.h" // Handle management

#include "Hooks.h" // Hook System

#include "Component.h" // Component type
#include "ComponentCreator.h" // Component Factory

#include "Object.h" // Object type

#include "SlotArray.h" // Slot array storage

#include "Message.h" // Messaging system

#include "Space.h" // Game Space type

#include "System.h" // Generic System/Interface type
#include "GameLogic.h" // GameLogic system

#include "Factory.h" // Object Factory
#include "Engine.h" // Core Engine

#include "Vec2.h" // Vector 2D
#include "Vec4.h" // Vector 4D

#include "DrawLib.h" // Drawing Extension Library

// Components
#include "CTester.h"
#include "CTransform.h"
#include "CSprite.h"
#include "CLuaComponent.h"

// Type and function Binding
#include "DefaultTypes.h"
#include "LuaDefaultBinds.h"






