/*****************************************************************
Filename: LuaInterface.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include <iostream>
#include <direct.h>
#include <stdlib.h>

namespace Framework
{
  namespace Lua
  {

    static const bool CrashOnLuaError = true;

    void LoadFile(lua_State* L, const char* name)
    {
      if (luaL_dofile(L, name) && CrashOnLuaError)
      {
        //ENGINE->LuaError(lua_tostring(L, -1));
        ErrorIf(true, "Lua Interface", "Error loading file:\n%s",
          lua_tostring(L, -1));
      }
    }

    void GenericToLua(lua_State* L, Variable& var)
    {
      // Establish a reference to a new set of userdata
      Variable* ref = (Variable*)lua_newuserdata(L, sizeof(Variable));
      new (ref) Variable(var); // copies stuff

      // Get the meta table
      luaL_getmetatable(L, var.GetTypeInfo()->LuaMetaTable());

      ErrorIf(lua_type(L, -1) != LUA_TTABLE, "Lua Interface",
        "META TABLE DOES NOT EXIST!");
 
      lua_setmetatable(L, -2);
    }

    void GenericFromLua(lua_State* L, int index, Variable* var)
    {
      //StackDump(L);

      ErrorIf(!lua_isuserdata(L, index), "Lua Interface",
        "Couldn't find self, try using :Func() and not .Func()");
      *var = *((Variable*)lua_touserdata(L, index));
    }

    int ErrorFunc(lua_State* L)
    {
      ENGINE->TraceLog.Log(TraceLevel::ERR, "Lua error: %s\n", lua_tostring(L, -1));
      //ErrorIf(true, "Lua Interface", "Lua error! %s", lua_tostring(L, -1));
      lua_pop(L, 1); // Pop off the stack
      return 0;
    }

    static void SetupTypeMembers(lua_State* L)
    {
      for (auto it = IntrospectionManager::Get()->GetTypeMap().begin();
        it != IntrospectionManager::Get()->GetTypeMap().end(); ++it)
      {
        // First check if the metatable already exists in lua
        // (It shouldn't! but just incase)
        luaL_getmetatable(L, it->second->LuaMetaTable());
        if (lua_isnil(L, 1))
        {
          // If we got a nil on the top of the stack then the meta table
          //doesnt exist and we pop the nil
          lua_pop(L, 1);

          // We are done with everything so we clear the stack
          lua_settop(L, 0);
        }
        else
        {
          // The meta table exists // 1
          //StackDump(L);

          // We want to make a table in the meta table for all the C++ members
          // inside of this data type. We use this for auto setters/getters
          // Create an entry in the metatable. Key="__members" Value={New Table}
          lua_pushstring(L, "__members"); // 2
          lua_createtable(L, 0, 0); // 3
          //StackDump(L);
          lua_settable(L, -3); // 1[2] = 3
          //StackDump(L);

          /* If this type has a special lua callback then we are going to ignore
           automating the serialization of it's members because we virtualize
           the data type inside of lua to cut back on the number of
           back and forth calls to and from lua.
           
           An example of this is the Vector#D types.
           We have a C++ math library for C++ vectors,
           and a lua math library for lua vectors.

           Whenever we send a vector to/from lua we convert it
           into the matching type.
           Example: C++ Vec2D -> Lua Vec2D */
          if (!it->second->HasToLuaCB())
          {
            // Get the value of the key "__members" on the metatable,
            // which is a table
            lua_getfield(L, -1, "__members"); // index 2
            //StackDump(L);

            // Iterate through each member, if it is a member
            // we want to create a setter/getter for then we do some stuff
            for (size_t i = 0; i < it->second->GetMembers().size(); ++i)
            {
              const Member* mem = &it->second->GetMembers()[i];

              if (mem->AutoLua())
              {
                // This member needs to have an automatic setter/getter
                // We want to create an entry in the __members table:
                // First we push the name of the member as the key
                // Then we push a constant member pointer as the value
                lua_pushstring(L, mem->Name()); // index 3
                it->second->ToLua(L, mem); // index 4
                //StackDump(L);
                lua_settable(L, -3); // 2[3] = 4
              }

            }
            //StackDump(L);
            // We are now done with dealing with automatic getters/setters
            // pop the __members table off of the stack
            lua_pop(L, 1);
            //StackDump(L);
          }


          lua_settop(L, 0);
          //StackDump(L);
        }
      }

      
    }

    static void SetupCTypes(lua_State* L)
    {
      lua_newtable(L);
      lua_setglobal(L, "_R");

      // Setup the lua-side stuff for all of our types
      for (auto it = IntrospectionManager::Get()->GetTypeMap().begin();
        it != IntrospectionManager::Get()->GetTypeMap().end(); ++it)
      {
        // First check if the metatable already exists in lua
        // (It shouldn't! but just incase)
        luaL_getmetatable(L, it->second->LuaMetaTable());
        if (lua_isnil(L, 1))
        {
          // If we got a nil on the top of the stack then the meta-table
          // doesnt exist and we pop the nil
          lua_pop(L, 1);

          // Get the global table: _R [Index 1 on stack]
          lua_getglobal(L, "_R"); // index 1

          // Create the metatable [Index 2 on stack]
          luaL_newmetatable(L, it->second->LuaMetaTable()); // index 2

          // Create an entry in the metatable. Key="__index" Value={MetaTable}
          lua_pushstring(L, "__index"); // index 3
          lua_pushvalue(L, -2); // 4
          lua_settable(L, -3); // 2[3] = 4

          lua_pushstring(L, it->second->Name()); // 3
          lua_pushvalue(L, -2); // 4
          lua_settable(L, -4); // 1[3] = 4

          // We are done with everything so we clear the stack
          lua_settop(L, 0);
        }
        else
        {
          // The meta table exists, this probably shouldn't happen,
          // pop the table
          lua_pop(L, 1);
        }
      }
    }

    static void GenerateComponentTable(lua_State* L)
    {
      // Creates a new table on the global table to act as a
      // database of components
      lua_newtable(L);
      lua_setglobal(L, "ComponentDB");

      // Gets the global table we just made
      lua_getglobal(L, "ComponentDB");
      // Iterate through each component type and push the name of
      // each component on and the ID
      for (unsigned i = 0; i < ecountComponents; ++i)
      {
        lua_pushstring(L, EnumComponent.m_literals[i].c_str());
        lua_pushnumber(L, i);
        lua_settable(L, -3);
      }
      lua_pop(L, 1);
    }

    lua_State* CreateEnvironment(void)
    {
      char cDirectory[512]; // buffer
      std::string lDirectory;
      _getcwd(cDirectory, sizeof(cDirectory));

      //lDirectory = "LUA_PATH=";
      lDirectory = cDirectory;
      lDirectory += "\\content";

      // Creates a new lua environment
      lua_State* L = luaL_newstate();
      
      // Push basic C++ meta table data
      SetupCTypes(L);

      // Initialization routine
      luaL_openlibs(L);
      luaopen_lfs(L); // Opens the file system

      // Set the working directory for lua
      lua_pop(L, -1); // Pop the stack
      SetPath(L, lDirectory.c_str());

      // Bind C++ library functions
      BindLibraryFunctions(L);

      // Initialize the Lua side interface
      LoadFile(L, "content/lua/engine/includes/interface.lua");

      // Generates a list of all components inside of lua
      GenerateComponentTable(L);

      // Tell Lua about the member variables and functions of C++ functions
      SetupTypeMembers(L);

      // Actually sets up the meta tables for C++ types
      Lua::CallFunc(L, "SetupCMetatables");

      // Tells the lua file system to load <all> the files in content/lua/
      Lua::CallFunc(L, "filesystem.LoadLuaFiles", "content/lua/");

      // Runs any post-initialization logic
      Lua::CallFunc(L, "LuaLoaded");

      // Binds C++ functions to lua
      BindDefaultFunctions(L);

      return L;
    }

    

    void CallMemberFunc(lua_State* L, Variable& var, const char* funcName)
    {
      CallMemberFuncFinal(L, var, funcName, nullptr, 0);
    }

    void CallMemberFuncFinal(lua_State* L, Variable& member,
      const char* funcName, Variable* args, size_t argCount)
    {
      lua_pushcfunction(L, ErrorFunc); // Stack 1
      luaL_getmetatable(L, member.GetTypeInfo()->LuaMetaTable()); // Stack 2
      lua_getfield(L, 2, funcName); // Stack 3
      member.ToLua(L); // Stack 4
      //StackDump(L); 

      for (size_t i = 0; i < argCount; ++i)
        args[i].ToLua(L);

      // Save the index of the error function
      int ErrorFuncIndex = -((int)(argCount + 4));

      //StackDump(L); 

      // Call pcall, which is a protected call which will run the
      // error function in case of execution error
      lua_pcall(L, argCount + 1, 1, ErrorFuncIndex);

      //StackDump(L); 

      // Pop the error function
      lua_remove(L, lua_gettop(L) - 2);
      //StackDump(L); 

      // Pop the meta table
      lua_remove(L, lua_gettop(L) - 1);

      lua_pop(L, 1); // pops nil
    }


    void CallFunc(lua_State* L, const char* funcName)
    {
      CallStaticFuncFinal(L, funcName, nullptr, 0);
    }

    void CallStaticFuncFinal(lua_State* L, const char* funcName,
      Variable* args, size_t argCount)
    {
      std::string module = funcName;
      std::string function;

      function = module.substr(module.find('.') + 1,
        module.length() - module.find('.') - 1);
      module = module.substr(0, module.find('.'));


      lua_pushcfunction(L, ErrorFunc); // Stack 1

      if (module.length() > 0 && module != function)
      {
        lua_getglobal(L, module.c_str()); // Stack 2

        //if (module == "gamespace" && function == "PauseGameSpace")
        //  StackDump(L);

        lua_getfield(L, -1, function.c_str()); // Stack 3


        for (size_t i = 0; i < argCount; ++i)
          args[i].ToLua(L);

        // Save the index of the error function
        int ErrorFuncIndex = -((int)(argCount + 3));

        // Call pcall, which is a protected call which will run the
        // error function in case of execution error
        lua_pcall(L, argCount, 1, ErrorFuncIndex);

        //StackDump(L);

        // Pop the error function
        lua_remove(L, lua_gettop(L) - 2);

        // Pop the module table
        lua_remove(L, lua_gettop(L) - 1);

        lua_pop(L, 1); // pops nil

        //StackDump(L);
      }
      else
      {
        lua_getglobal(L, funcName); // Stack 2

        for (size_t i = 0; i < argCount; ++i)
          args[i].ToLua(L);

        // Save the index of the error function
        int ErrorFuncIndex = -((int)(argCount + 2));

        // Call pcall, which is a protected call which will run the
        // error function in case of execution error
        lua_pcall(L, argCount, 1, ErrorFuncIndex);

        // Pop the error function
        lua_remove(L, lua_gettop(L) - 1);

        lua_pop(L, 1); // pops nil
      }
      
    }

    void StackDump(lua_State* L)
    {
      int top = lua_gettop(L);

      std::cout << "\n=== Lua Stack Dump ===\n" << "   Sizeof stack: "
        << top << "\n";

      for (int i = 1; i <= top; ++i)
      {
        int type = lua_type(L, i);
        switch (type)
        {
        case LUA_TSTRING:
          std::cout << "  "<<i<<": \""<<lua_tostring(L, i)<<"\"\n";
          break;
        case LUA_TBOOLEAN:
          std::cout << "  "<<i<<": "<<lua_toboolean(L, i)<<"\n";
          break;
        case LUA_TNUMBER:
          std::cout << "  "<<i<<": "<<lua_tonumber(L, i)<<"\n";
          break;
        default:
          std::cout << "  "<<i<<": "<<lua_typename(L, type)<<"\n";
          break;
        }
      }

      std::cout << "=== End Lua Dump ===\n";

    }



    int GenericFunc(lua_State* L)
    {

      // Create a pointer to the function as a function bind
      Function* fn = (Function*)lua_touserdata(L, lua_upvalueindex(1));
      assert(fn);

      int argCount = lua_gettop(L);

      if (fn->IsMethod())
      {
        ErrorIf(fn->Signature()->ArgCount() + 1 != argCount,
          "Lua Interface", "Argument count wrong!");
      }
      else
      {
        ErrorIf(fn->Signature()->ArgCount() != argCount,
          "Lua Interface", "Argument count wrong!");
      }

      // Lets check to see if the function is going to return anything
      const TypeInfo* rType = fn->Signature()->GetRet();
      bool returns = rType ? true : false;

      Variable ret;

      if (returns)
      {
        // Allocate some space for the return value
        ret = Variable(fn->Signature()->GetRet(),
          alloca(fn->Signature()->GetRet()->Size()));
        // Run the default constructor on the return type
        ret.PlacementNew();
      }

      // Allocate space for the arguments and placement new on each one
      Variable* stackArgs = (Variable*)alloca(sizeof(Variable)
        * fn->Signature()->ArgCount());

      // iterate through all of them, placement new
      for (size_t i = 0; i < fn->Signature()->ArgCount(); ++i)
      {
        // Todo research
        new (stackArgs + i) Variable(fn->Signature()->GetArg(i),
          alloca(fn->Signature()->GetArg(i)->Size()));
        stackArgs[i].PlacementNew();
      }

      if (fn->IsMethod())
      {
        // Load the context from the lua
        fn->Signature()->GetContext()->FromLua(L, 1, &fn->Context());

        // Ensure we are using a valid context
        ErrorIf(!fn->Context().IsValid(), "Lua Interface",
          "Invalid binding: meta does not match for type %s",
          fn->Signature()->GetContext()->Name());

        // Load all the arguments from the stack
        for (size_t i = 0; i < fn->Signature()->ArgCount(); ++i)
        {
          // The first args are context, so we ignore those
          stackArgs[i].FromLua(L, i + 2);
        }
      }
      else // Not a method
      {
        // Load all the arguments from the stack
        for (size_t i = 0; i < fn->Signature()->ArgCount(); ++i)
        {
          stackArgs[i].FromLua(L, i + 1);
        }
      }

      //StackDump(L);

      // Call the function
      (*fn)(ret, stackArgs, fn->Signature()->ArgCount());

      // Run destructors on the arguments now that we are one
      for (size_t i = 0; i < fn->Signature()->ArgCount(); ++i)
      {
        //_freea((stackArgs[i].GetData()));
        stackArgs[i].PlacementDelete();
      }

      //_freea(stackArgs);

      if (returns)
      {
        ret.ToLua(L);
        // destructor for the return value
        ret.PlacementDelete();

        //_freea(ret.GetData());

        return 1;
      }

      //StackDump(L);

      //int a = lua_gettop(L) - fn->Signature()->ArgCount();

      return 1; //;
    }

    void BindFunctionToLua(lua_State* L, Function* fn,
      const char* name, const char* table)
    {
      if (table != nullptr)
      {
        lua_getglobal(L, table);
        lua_pushstring(L, name);
        lua_pushlightuserdata(L, fn);
        lua_pushcclosure(L, GenericFunc, 1);
        //StackDump(L);
        // Insert the function into the 1[2] = 3
        lua_settable(L, -3);

        // Set the top of th stack
        lua_settop(L, 0);

        return;
      }
      // First check if the function is a member function
      if (fn->IsMethod())
      {
        // Get the metatable from lua
        luaL_getmetatable(L, fn->Signature()->GetContext()->LuaMetaTable()); // 1
        // Push the name of the function
        lua_pushstring(L, name); // 2
        // Push user data (the function pointer)
        lua_pushlightuserdata(L, fn); // 3
        // Push our generic function onto the stack TODO: research
        lua_pushcclosure(L, GenericFunc, 1);
        // Insert the function into the 1[2] = 3
        lua_settable(L, -3);

        // Set the top of th stack
        lua_settop(L, 0);
      }
      else
      {
        // Push the function pointer
        lua_pushlightuserdata(L, fn);
        lua_pushcclosure(L, Lua::GenericFunc, 1);
        lua_setglobal(L, name);
      }

    }

    void CreateNewGTable(lua_State* L, const char* name)
    {
      lua_getglobal(L, name);
      if (lua_isnil(L, 1))
      {
        lua_newtable(L);
        lua_setglobal(L, name);
      }
      else
        lua_pop(L, 1);
      
    }

    int SetPath(lua_State* L, const char* path)
    {
      lua_getglobal(L, "package"); // -1
      // Gets the path field from the top of the stack -2
      lua_getfield(L, -1, "path"); 
      // Grab the string from the stack
      std::string cur_path = lua_tostring(L, -1); 
      //cur_path.append(1,';');
      //cur_path.append(path);
      lua_pop(L, 1); // Get rid of the string on the stack -1
      lua_pushstring(L, path); // push the string -2
      // Set the field "path" in the table at -2 with the value at the top
      lua_setfield(L, -2, "path"); 
      lua_pop(L,1); // Pop off the package table from the stack
      return 0;
    }


    void Shutdown(lua_State* L)
    {
      lua_close(L);
    }

  }
}