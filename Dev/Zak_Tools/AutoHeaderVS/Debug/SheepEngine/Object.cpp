/*****************************************************************
Filename: Object.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Object.h"
#include <algorithm>
#include <functional>
#include <iterator>

namespace Framework
{

  //Binary search a sorted array of Objects
  static GameObject* BinaryChildSearch(GameSpace* space, ChildArray& children, size_t uid)
  {
    
    size_t begin = 0;
    size_t end = children.size();

    while(begin < end)
    {
      size_t mid = (begin + end) / 2;
      if (space->GetHandles().GetAs<GameObject>(children[mid])->guid < uid)
        begin = mid + 1;
      else
        end = mid;
    }

    if((begin < children.size()) && (space->GetHandles().GetAs<GameObject>(children[begin])->guid == uid))
      return space->GetHandles().GetAs<GameObject>(children[begin]);
    else
      return NULL;
      
  }

  /// <summary>
  /// Marks the object for deletion
  /// </summary>
  void GameObject::Destroy()
  {
    m_active = false;
  }

  GameObject::GameObject()
    : Generic(eGameObject),
    archetype(),
    name()
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
      m_components[i] = Handle::null;
    fastChildSearch = false;
  }


  /// <summary>
  /// Removes all components from the game object
  /// </summary>
  GameObject::~GameObject()
  {
    // If the object is not active, time to destroy it
    for (unsigned int j = 0; j < ecountComponents; ++j)
    {
      // Get the enum type of the component
      EComponent type = (EComponent)j;

      // Check to see if the Object has that type of component
      if (HasComponent(type) && type != eLuaComponent)
      {
        // Get the component
        GameComponent* comp = GetComponent(type);
        comp->Remove(); // Remove the component

        // Remove the handle from the space
        space->GetHandles().Remove(comp->self);

        // Deconstruct the game component
        comp->~GameComponent();

        // Free the component and update any handles
        GameComponent* moved = (GameComponent*)space->GetComponents(type)->Free(comp);
        if (moved)
          space->GetHandles().Update(moved, moved->self);
      }
    } // End component loop

    for (unsigned int j = 0; j < m_luaComponents.size(); ++j)
    {
      // Get the component
      LuaComponent* comp = GetLuaComponent(j);
      comp->Remove(); // Call the remove routine

      // Remove the handle from the space
      space->GetHandles().Remove(comp->self);

      // Free the component and update any handles
      GameComponent* moved = (GameComponent*)space->GetComponents(eLuaComponent)->Free(comp);
      if (moved)
        space->GetHandles().Update(moved, moved->self);

    } // End component loop

    name.~basic_string();
    archetype.~basic_string();
  }

  
  /// <summary>
  /// Initializes each of the objects components separate from the
  /// constructor initialization.
  /// </summary>
  void GameObject::Initialize()
  {
    GameComponent* component;
    for (size_t i = 0; i < ecountComponents; ++i)
    {
      if (m_components[i] == Handle::null)
        continue;
      component = space->GetHandles().GetAs<GameComponent>(m_components[i]);
      component->owner = self;
      component->Initialize();
    }

    if (space->GetName().length() > 0)
      for (size_t i = 0; i < m_luaComponents.size(); ++i)
      {
        component = space->GetHandles().GetAs<GameComponent>(m_luaComponents[i]);
        component->owner = self;
        component->Initialize();
      }
  }

  bool GameObject::ObjectSorter(Handle left, Handle right)
  {
    return space->GetHandles().GetAs<GameObject>(left)->guid > space->GetHandles().GetAs<GameObject>(right)->guid;
  }

  /// <summary>
  /// Adds the component.
  /// </summary>
  /// <param name="component">The component.</param>
  void GameObject::AddComponent(GameComponent* component)
  {
    // Set the component's owner to ourself
    component->owner = self;
    if (component->typeID == eLuaComponent)
      m_luaComponents.push_back(component->self);
    else
      m_components[component->typeID] = component->self;
  }

  /// <summary>
  /// Gets the child.
  /// </summary>
  /// <param name="uid">The uid.</param>
  /// <returns></returns>
  GameObject* GameObject::GetChild(size_t uid)
  {
    if (!fastChildSearch)
      return NULL;

    return BinaryChildSearch(space, m_children, uid);
  }

  /// <summary>
  /// Adds the child.
  /// </summary>
  /// <param name="obj">The object.</param>
  void GameObject::AddChild(Handle obj)
  {
    m_children.push_back(obj);

    if (!fastChildSearch)
      return;
    // Sort the child array so binary search can be used to find children quickly
    std::sort(m_children.begin(), m_children.end(), std::bind(&GameObject::ObjectSorter, this, std::placeholders::_1, std::placeholders::_2) );
  }

  /// <summary>
  /// Sets the parent.
  /// </summary>
  /// <param name="obj">The object.</param>
  void GameObject::SetParent(Handle obj)
  {
    // Set the parent
    m_parent = obj;

    // Add the child onto the parent
    space->GetHandles().GetAs<GameObject>(obj)->AddChild(self);
  }

  bool GameObject::HasComponent( EComponent type ) const
  {
    return m_components[type] != Handle::null;
  }

  bool GameObject::HasComponent( size_t type ) const
  {
    return m_components[type] != Handle::null;
  }

  Handle GameObject::GetComponentHandle(const char* type)
  {
    if (GET_ENUM(Component)->IsAnEntry(type))
    {
      EComponent i = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
      return GetComponentHandle(i);
    }

    return Handle::null;
  }

  Handle GameObject::GetComponentHandle(EComponent type)
  {
    return space->GetHandles().GetAs<GameComponent>(m_components[type])->self;
  }

  GameComponent* GameObject::GetComponent(const char *type)
  {
    if (GET_ENUM(Component)->IsAnEntry(type))
    {
      EComponent i = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
      return GetComponent(i);
    }

    return NULL;
  }

  GameComponent* GameObject::GetComponent(unsigned int type)
  {
    return space->GetHandles().GetAs<GameComponent>(m_components[type]);
  }

  LuaComponent* GameObject::GetLuaComponent(unsigned int index)
  {
    return space->GetHandles().GetAs<LuaComponent>(m_luaComponents[index]);
  }

  void GameObject::Serialize(File& file, Variable var)
  {
    GameObject *o = &var.GetValue<GameObject>( );
    Serializer *s = Serializer::Get( );
    const TypeInfo* info = var.GetTypeInfo();
    int& pad = s->GetPadLevel( );

    s->Padding( file, pad );
    file.Write( "GameObject\n" );
    s->Padding( file, pad++ );
    file.Write( "{\n" );
    //s->Padding( file, pad );

    // Iterate through the members we need to serialize
    for(unsigned i = 0; i < info->GetMembers().size(); ++i)
    {
      // Get a pointer to the member
      const Member* member = &info->GetMembers().front() + i;
      // Add some padding in
      s->Padding( file, pad );

      // Get a pointer to the member type
      const TypeInfo* memberInfo = member->Type();
      // Write the name of the member
      file.Write("%s ", member->Name());

      // Get a pointer to the location of the member inside the data
      // struct/class/whatever itself
      void* offsetData = PTR_ADD(o, member->Offset());

      // Construct a new variable out of that, and then serialize that variable
      // yay recursive
      memberInfo->Serialize(file, Variable(memberInfo, offsetData));
    }

    //s->Padding( file, pad );

    //const TypeInfo* typeInfo = GET_TYPE(Handle);

    Serializer::Get()->SetUserData(o->space);
    // Now we are going to iterate through every component and serialize them
    for ( unsigned int i = 0; i < ecountComponents; ++i)
    {

      
      if (o->HasComponent(EComponent(i)) && i != eLuaComponent)
      {
        s->Padding( file, pad );

        Variable v = o->m_components[i];
        v.Serialize(file);
      }

      if(i != ecountComponents - 1)
      {
        file.Write( "\n" );
      }
    }

    // Serialize lua components
    info = GET_TYPE(LuaComponent);
    for (unsigned int i = 0; i < o->m_luaComponents.size(); ++i)
    {
      LuaComponent* LC = o->space->GetHandles().GetAs<LuaComponent>(o->m_luaComponents[i]);

      LC->QueryLoadCommand();

      s->Padding(file, pad);
      Variable v(info, LC);
      v.Serialize(file);

      if (i != o->m_luaComponents.size() - 1)
      {
        file.Write("\n");
      }

    }

    Serializer::Get()->SetUserData(nullptr);

    // We are all done so close the bracket
    s->Padding( file, --pad );
    file.Write( "}\n" );
  }

  void GameObject::Deserialize(File& file, Variable var)
  {
    Serializer* s = Serializer::Get();
    GameObject* obj = &var.GetValue<GameObject>();
    fpos_t lastcomp;

    // Peak into the file and see if we can recognize a data type
    const TypeInfo* info = Serializer::Get()->PeekType(file);

    // Make sure it is indeed an object
    ErrorIf(var.GetTypeInfo( ) != info, "GameObject deserialization", "Invalid type found");

    // Our peek function was nice enough to figure out the starting level for us
    int startLevel = Serializer::Get()->GetPadLevel();

    for (unsigned int i = 0; i < info->GetMembers().size(); ++i)
    {
      const Member* mem = s->PeekMember(file, info->GetMembers(), startLevel);

      // If we found a member then we can turn it into a variable type and then
      // deserialize that variable into to data we can use
      if (mem)
      {
        // Create a Variable out of the member we found, we need to offset the
        // start position of the variable by the start of our current variable
        // by the offset from the member and the start of the current variable
        Variable member( mem->Type(), PTR_ADD(var.GetData(), mem->Offset()) );

        // Now that we have the Variable, lets deserialize it
        member.Deserialize(file);
      }
    }
    
    // Now that we have read in any additional members from the GameObject
    // We can move onto reading in the components
    fgetpos(file.fp, &lastcomp);
    for(;;)
    {
      GameComponent* comp = FACTORY->DeserializeComponent(file, obj->space);

      if (!comp)
        break; // No component found

      obj->AddComponent(comp);
      fgetpos(file.fp, &lastcomp);
    }

    // We read in all the components, initialize the object
    obj->Initialize();

    // Sometimes we are reading an object in from the middle of a level file
    // so we need to return the file pointer back to the last } we encountered
    if (file.Validate())
    {
      fsetpos(file.fp, &lastcomp);
      file.GetLine("}");
    }
  }

  //GameObject& GameObject::operator=(const GameObject& rhs)
  //{
  //  for (size_t i = 0; i < ecountComponents; ++i)
  //  {
  //    if (rhs.HasComponent(i) && i != eLuaComponent)
  //    {

  //    }
  //  }
  //}


  void GameObject::LuaGetComponent(size_t type)
  {
    if (!HasComponent(EComponent(type)))
    {
      return;
    }
    Variable var(GET_STR_TYPE(EnumComponent.m_literals[type].c_str()), space->GetHandles().Get(m_components[type]));

    Lua::GenericToLua(ENGINE->Lua(), var);

    return;
  }

};