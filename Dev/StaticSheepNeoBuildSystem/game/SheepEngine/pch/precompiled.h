
//#include "../../src/API.h"

#include "config/Config.h"

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

namespace SheepFizz
{
  typedef void* ExternalManifold;
}

// Pre Reqs
#include <assert.h>

// Utilities
#include "modules/introspection/Qualifiers.h" // Qualifier removal
#include "engine/core/Macros.h" // Macro Helpers
#include "Utilities.h" // Debug macros and helpers


#include "modules/serializer/file/File.h" // File handler
#include "modules/introspection/variable/Variable.h" // Variable delegate
#include "modules/introspection/enum/Enum.h" // Enumeration Serialization/Introspection
#include "modules/introspection/types/TypeInfo.h" // Type introspection
#include "modules/serializer/Serialization.h" // Serialization
#include "modules/serializer/PODSerialization.h" // Plain old Data serialization routines

#include "modules/functionbinding/FuncSignature.h" // Function Signatures (For Binding)
#include "modules/functionbinding/Function.h" // Function binding

#include "modules/introspection/manager/IntrospectionManager.h" // Introspection

#include "modules/lua/transfer/LuaTransfer.h" // Lua<->C++ Transfer
#include "modules/lua/interface/LuaInterface.h" // Lua interface

#include "types/handle/Handle.h" // Handle type

#include "modules/memory/ObjectAllocator.h" // Object Memory manager
#include "types/generic/Generic.h" // Generic Object/Component type

#include "modules/handlemanager/HandleManager.h" // Handle management

#include "modules/hooks/Hooks.h" // Hook System

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
#include "modules/introspection/DefaultTypes.h"
#include "modules/lua/LuaDefaultBinds.h"

#include "modules/serializer/file/File.h"
#include "modules/introspection/variable/Variable.h"




