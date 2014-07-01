/*****************************************************************
Filename: Object.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
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
    // @TODO: Tell the object manager (Space?Layer?) to delete this when possible
  }

  GameObject::GameObject()
    : Generic(eGameObject)
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
      m_components[i] = Handle::null;
    fastChildSearch = false;
  }

  GameObject::~GameObject()
  {
    // @TODO: Delete every component
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
      component = space->GetHandles().GetAs<GameComponent>(m_components[i]);
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

  bool GameObject::HasComponent( EComponent type )
  {
    return m_components[type] != Handle::null;
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

    s->Padding( file, pad );

    //const TypeInfo* typeInfo = GET_TYPE(Handle);

    Serializer::Get()->SetUserData(o->space);
    // Now we are going to iterate through every component and serialize them
    for(unsigned i = 0; i < ecountComponents; ++i)
    {
      if (o->HasComponent(EComponent(i)))
      {
        Variable v = o->m_components[i];
        v.Serialize(file);
      }
      //// Find the component type
      //EComponent type = (EComponent)i;
      //if (o->HasComponent( type ))
      //{
      //  // If the object has the specified component, then get the type info
      //  const TypeInfo *typeInfo = FACTORY->GetComponentType( type );
      //  // Create a variable from the component
      //  Variable v( typeInfo, o->GetComponent( type ) );
      //  // Serialize the component into the file
      //  v.Serialize( file );

      //  // If we aren't quite yet at the end of all the components, make a new line
      //  // And pad it for any future components
      if(i != ecountComponents - 1)
      {
        file.Write( "\n" );
        s->Padding( file, pad );
      }
      //}
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

};