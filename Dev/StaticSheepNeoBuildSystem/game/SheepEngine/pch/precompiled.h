
//#include "../../src/API.h"
#define USE_ANTTWEAKBAR 1



#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else  // _DEBUG
#define DBG_NEW new
#endif

extern "C"
{
#include "lua.h"
#include "lua.hpp"
#include "lauxlib.h"
#include "luaconf.h"
#include "lualib.h"
#include "lfs.h"
}

// Pre Reqs
#include <assert.h>

// Utilities
#include "engine/introspection/Qualifiers.h" // Qualifier removal
#include "engine/core/Macros.h" // Macro Helpers
#include "Utilities.h" // Debug macros and helpers


#include "engine/serializer/file/File.h" // File handler
#include "engine/introspection/variable/Variable.h" // Variable delegate
#include "engine/introspection/enum/Enum.h" // Enumeration Serialization/Introspection
#include "engine/introspection/types/TypeInfo.h" // Type introspection
#include "engine/serializer/Serialization.h" // Serialization
#include "engine/serializer/PODSerialization.h" // Plain old Data serialization routines

#include "engine/functionbinding/FuncSignature.h" // Function Signatures (For Binding)
#include "engine/functionbinding/Function.h" // Function binding

#include "engine/introspection/manager/IntrospectionManager.h" // Introspection

#include "engine/lua/transfer/LuaTransfer.h" // Lua<->C++ Transfer
#include "engine/lua/interface/LuaInterface.h" // Lua interface

#include "types/handle/Handle.h" // Handle type

#include "memory/ObjectAllocator.h" // Object Memory manager
#include "types/generic/Generic.h" // Generic Object/Component type

#include "engine/handlemanager/HandleManager.h" // Handle management

#include "engine/hooks/Hooks.h" // Hook System

#include "components/base/Component.h" // Component type
#include "engine/factory/ComponentCreator.h" // Component Factory

#include "types/object/Object.h" // Object type
#include "types/archetype/Archetype.h" // Object Archetype (Virtual type)

#include "types/slotarray/SlotArray.h" // Slot array storage

#include "types/message/Message.h" // Messaging system

#include "types/space/Space.h" // Game Space type

#include "systems/System.h" // Generic System/Interface type
#include "systems/gamelogic/GameLogic.h" // GameLogic system

#include "engine/framerate/FramerateController.h" // frames
#include "engine/factory/Factory.h" // Object Factory
#include "engine/core/Engine.h" // Core Engine


#include "types/vectors/Vec2.h" // Vector 2D
#include "types/vectors/Vec3.h" // Vector 3D
#include "types/vectors/Vec4.h" // Vector 4D

#include "systems/graphics/DrawLib.h" // Drawing Extension Library

// Type and function Binding
#include "engine/introspection/DefaultTypes.h"
#include "engine/lua/LuaDefaultBinds.h"

#include "engine/serializer/file/File.h"
#include "engine/introspection/variable/Variable.h"







