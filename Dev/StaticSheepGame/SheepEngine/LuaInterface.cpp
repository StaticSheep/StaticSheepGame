/*****************************************************************
Filename: LuaInterface.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

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
        ErrorIf(true, "Lua Interface", "Error loading file:\n%s", lua_tostring(L, -1));
      }
    }

    void GenericToLua(lua_State* L, Variable& var)
    {
      // Establish a reference to a new set of userdata
      Variable* ref = (Variable*)lua_newuserdata(L, sizeof(Variable));
      new (ref) Variable(var); // copies stuff

      // Get the meta table
      luaL_getmetatable(L, var.GetTypeInfo()->LuaMetaTable());

      ErrorIf(lua_type(L, -1) != LUA_TTABLE, "Lua Interface", "META TABLE DOES NOT EXIST!");
 
      lua_setmetatable(L, -2);
    }

    void GenericFromLua(lua_State* L, int index, Variable* var)
    {
      //StackDump(L);

      ErrorIf(!lua_isuserdata(L, index), "Lua Interface", "Couldn't find self, try using :Func() and not .Func()");
      *var = *((Variable*)lua_touserdata(L, index));
    }

    int ErrorFunc(lua_State* L)
    {
      ErrorIf(true, "Lua Interface", "Lua error! %s", lua_tostring(L, -1));
      lua_pop(L, 1); // Pop off the stack
      return 0;
    }

    lua_State* CreateEnvironment(void)
    {
      char cDirectory[512]; // buffer
      std::string lDirectory;
      _getcwd(cDirectory, sizeof(cDirectory));

      lDirectory = "LUA_PATH=";
      lDirectory += cDirectory;
      lDirectory += "\\content";

      _putenv(lDirectory.c_str());

      // Creates a new lua environment
      lua_State* L = luaL_newstate();

      

      // Initialization routine
      luaL_openlibs(L);
      luaopen_lfs(L); // Opens the file system

      lua_pop(L, -1); // Pop the stack

      //SetPath(L, Directory);
      
      LoadFile(L, "content/lua/engine/includes/interface.lua");

      // Setup the __index method for meta tables (FUCK YEAH TABLES)
      for (auto it = IntrospectionManager::Get()->GetTypeMap().begin(); it != IntrospectionManager::Get()->GetTypeMap().end(); ++it)
      {
        luaL_getmetatable(L, it->second->LuaMetaTable());
        if (lua_isnil(L, 1))
        {
          lua_pop(L, 1);

          lua_getglobal(L, "_R"); // index 1

          luaL_newmetatable(L, it->second->LuaMetaTable()); // index 2

          lua_pushstring(L, "__index"); // index 3
          lua_pushvalue(L, -2); // 4
          lua_settable(L, -3); // 2[3] = 4

          

          lua_pushstring(L, it->second->LuaMetaTable()); // 3
          lua_pushvalue(L, -2); // 4
          lua_settable(L, -4); // 2[3] = 4

          lua_settop(L, 0);
        }
        else
        {
          lua_pop(L, 1);
        }
      }

      Lua::CallFunc(L, "SetupMetatables");

      CallFunc(L, "filesystem.LoadLuaFiles", "content/lua/");

      return L;
    }

    void CallMemberFunc(lua_State* L, Variable& var, const char* funcName)
    {
      CallMemberFuncFinal(L, var, funcName, nullptr, 0);
    }

    void CallMemberFuncFinal(lua_State* L, Variable& member, const char* funcName, Variable* args, size_t argCount)
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

    void CallStaticFuncFinal(lua_State* L, const char* funcName, Variable* args, size_t argCount)
    {
      std::string module = funcName;
      std::string function;

      function = module.substr(module.find('.') + 1, module.length() - module.find('.') - 1);
      module = module.substr(0, module.find('.'));


      lua_pushcfunction(L, ErrorFunc); // Stack 1

      if (module.length() > 0 && module != function)
      {
        lua_getglobal(L, module.c_str()); // Stack 2

        lua_getfield(L, 2, function.c_str()); // Stack 3


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

      std::cout << "\n=== Lua Stack Dump ===\n" << "   Sizeof stack: " << top << "\n";

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
        ErrorIf(fn->Signature()->ArgCount() + 1 != argCount, "Lua Interface", "Argument count wrong!");
      }
      else
      {
        ErrorIf(fn->Signature()->ArgCount() != argCount, "Lua Interface", "Argument count wrong!");
      }

      // Lets check to see if the function is going to return anything
      const TypeInfo* rType = fn->Signature()->GetRet();
      bool returns = rType ? true : false;

      Variable ret;

      if (returns)
      {
        // Allocate some space for the return value
        ret = Variable(fn->Signature()->GetRet(), alloca(fn->Signature()->GetRet()->Size()));
        // Run the default constructor on the return type
        ret.PlacementNew();
      }

      // Allocate space for the arguments and placement new on each one
      Variable* stackArgs = (Variable*)alloca(sizeof(Variable) * fn->Signature()->ArgCount());

      // iterate through all of them, placement new
      for (size_t i = 0; i < fn->Signature()->ArgCount(); ++i)
      {
        // Todo research
        new (stackArgs + i) Variable(fn->Signature()->GetArg(i), alloca(fn->Signature()->GetArg(i)->Size()));
        stackArgs[i].PlacementNew();
      }

      if (fn->IsMethod())
      {
        // Load the context from the lua
        fn->Signature()->GetContext()->FromLua(L, 1, &fn->Context());

        // Ensure we are using a valid context
        ErrorIf(!fn->Context().IsValid(), "Lua Interface", "Invalid binding: meta does not match for type %s", fn->Signature()->GetContext()->Name());

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

      // Call the function
      (*fn)(ret, stackArgs, fn->Signature()->ArgCount());

      // Run destructors on the arguments now that we are one
      for (size_t i = 0; i < fn->Signature()->ArgCount(); ++i)
        stackArgs[i].PlacementDelete();

      if (returns)
      {
        ret.ToLua(L);
        // destructor for the return value
        ret.PlacementDelete();
        return 1;
      }

      return 0;
    }

    void BindFunctionToLua(lua_State* L, Function* fn, const char* name, const char* table)
    {
      if (table != nullptr)
      {
        lua_getglobal(L, table);
        lua_pushstring(L, name);
        lua_pushlightuserdata(L, fn);
        lua_pushcclosure(L, GenericFunc, 1);
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
      lua_newtable(L);
      lua_setglobal(L, name);
    }

    int SetPath(lua_State* L, const char* path)
    {
      lua_getglobal(L, "package"); // -1
      lua_getfield(L, -1, "path"); // Gets the path field from the top of the stack -2
      std::string cur_path = lua_tostring(L, -1); // Grab the string from the stack
      cur_path.append(1,';');
      cur_path.append(path);
      lua_pop(L, 1); // Get rid of the string on the stack -1
      lua_pushstring(L, cur_path.c_str()); // push the string -2
      lua_setfield(L, -2, "path"); // Set the field "path" in the table at -2 with the value at the top
      lua_pop(L,1); // Pop off the package table from the stack
      return 0;
    }


    void Shutdown(lua_State* L)
    {
      lua_close(L);
    }

  }
}