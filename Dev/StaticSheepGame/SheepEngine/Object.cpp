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

#include "AntTweakModule.h"

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
    name(),
    tweakLookup(nullptr),
    tweakCCompCallbacks(nullptr),
    tweakLuaCompCallbacks(nullptr)
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

    if (tweakHandle != Handle::null)
    {
      AntTweak::TBar* objBar = ATWEAK->GetBar(tweakHandle);
      ATWEAK->RemoveBar(objBar);
    }

    if (tweakLookup)
    {
      delete tweakLookup;
      tweakLookup = nullptr;
    }

    if (tweakCCompCallbacks)
    {
      for (size_t i=0; i < tweakCCompCallbacks->size(); ++i)
      {
        delete (*tweakCCompCallbacks)[i];
      }
      delete tweakCCompCallbacks;
      tweakCCompCallbacks = nullptr;
    }
    

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



  /// <summary>
  /// Determines whether the specified object has component.
  /// </summary>
  /// <param name="type">The type.</param>
  /// <returns></returns>
  bool GameObject::HasComponent( EComponent type ) const
  {
    return m_components[type] != Handle::null;
  }


  /// <summary>
  /// Determines whether the specified object has component.
  /// </summary>
  /// <param name="type">The type.</param>
  /// <returns></returns>
  bool GameObject::HasComponent( size_t type ) const
  {
    return m_components[type] != Handle::null;
  }


  /// <summary>
  /// Gets the component handle of a type from the object
  /// </summary>
  /// <param name="type">The type.</param>
  /// <returns></returns>
  Handle GameObject::GetComponentHandle(const char* type)
  {
    if (GET_ENUM(Component)->IsAnEntry(type))
    {
      EComponent i = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
      return GetComponentHandle(i);
    }

    return Handle::null;
  }


  /// <summary>
  /// Gets the component handle of a type from the object
  /// </summary>
  /// <param name="type">The type.</param>
  /// <returns></returns>
  Handle GameObject::GetComponentHandle(EComponent type)
  {
    return space->GetHandles().GetAs<GameComponent>(m_components[type])->self;
  }

  /// <summary>
  /// Gets a component
  /// </summary>
  /// <param name="type">The type.</param>
  /// <returns></returns>
  GameComponent* GameObject::GetComponent(const char *type)
  {
    if (GET_ENUM(Component)->IsAnEntry(type))
    {
      EComponent i = (EComponent)GET_ENUM(Component)->GetIndexFromString(type);
      return GetComponent(i);
    }

    return NULL;
  }

  /// <summary>
  /// Gets a component.
  /// </summary>
  /// <param name="type">The type.</param>
  /// <returns></returns>
  GameComponent* GameObject::GetComponent(unsigned int type)
  {
    return space->GetHandles().GetAs<GameComponent>(m_components[type]);
  }

  /// <summary>
  /// Gets a component. [LUA BIND]
  /// </summary>
  /// <param name="index">The index.</param>
  /// <returns></returns>
  LuaComponent* GameObject::GetLuaComponent(unsigned int index)
  {
    return space->GetHandles().GetAs<LuaComponent>(m_luaComponents[index]);
  }


  /*--------------------------------------------------------------------------

  Serialization

  --------------------------------------------------------------------------*/

  /// <summary>
  /// Serializes the object
  /// </summary>
  /// <param name="file">The file.</param>
  /// <param name="var">The variable.</param>
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


  /// <summary>
  /// Deserializes a file into an object
  /// </summary>
  /// <param name="file">The file.</param>
  /// <param name="var">The variable.</param>
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

  /*--------------------------------------------------------------------------

  Tweaking

  --------------------------------------------------------------------------*/

  /// <summary>
  /// Custom tweaking routine for objects. Instead of the object being tweaked on the specified bar,
  /// a new bar is created for the object to be tweaked upon. The bar will be filled out with the
  /// properties of the object itself, as well as the tweakable variables from the components which
  /// the object owns.
  /// </summary>
  /// <param name="bar">The bar. [UN-USED}</param>
  /// <param name="var">Variable delegate for the object</param>
  /// <param name="tempLabel">The temporary label.</param>
  /// <param name="label">The actual label.</param>
  void GameObject::CustomTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
  {
    // Figure out what to call the bar for tweaking game object
    GameObject& obj = var.GetValue<GameObject>();

    // If the object already has a tweak handle then we can just update the tweak bar
    if (obj.tweakHandle != Handle::null)
    {
      obj.UpdateTweakBar();
      return;
    }

    // Decide a name
    std::string objName(obj.name);
    objName += " [GUID: ";
    objName += std::to_string(obj.guid) + "]";

    // Create a new bar for this object
    AntTweak::TBar* objectBar = ATWEAK->CreateBar(objName.c_str());
    obj.tweakHandle = objectBar->self;
    obj.tweakListComponents = false;
    objectBar->SetIconify(false);

    obj.UpdateTweakBar();
  }


  /// <summary>
  /// Updates the archetype.
  /// </summary>
  /// <param name="clientData">The client data.</param>
  static void UpdateArchetype(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);

    if (obj->archetype.length() > 0)
      FACTORY->SaveObjectToArchetype(obj, obj->archetype.c_str());
  }


  /// <summary>
  /// Button callback for adding a component list
  /// </summary>
  /// <param name="clientData">The client data.</param>
  static void ShowAddComponentList(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);

    obj->tweakListComponents = !obj->tweakListComponents;

    if (obj->tweakCCompCallbacks == nullptr)
    {
      obj->tweakCCompCallbacks = DBG_NEW std::vector<TweakObjComp*>();

      // Load up the C++ Component callback tables
      for(size_t i=0; i < ecountComponents - 1; ++i)
      {
        TweakObjComp* userdata = DBG_NEW TweakObjComp();
        userdata->gl.self = obj->self;
        userdata->gl.space = obj->space;
        userdata->compType = (EComponent)i;

        obj->tweakCCompCallbacks->push_back(userdata);
      }

      // Load up the Lua component callback tables
      //TODO
    }

    obj->UpdateTweakBar();
    
  }

  /// <summary>
  /// Callback for AntTweak bar adding a component on an object
  /// </summary>
  /// <param name="clientData">The client data.</param>
  static void TweakAddComponent(void* clientData)
  {
    TweakObjComp* oc = (TweakObjComp*)clientData;
    GameObject* obj = oc->gl.space->GetGameObject(oc->gl.self);
    EComponent cType = oc->compType;
    std::string luaType = oc->luaCompName;


    if (cType < eLuaComponent)
    {
      // Create the component and initialize it
      GameComponent* nComp = oc->gl.space->CreateComponent(cType);
      obj->AddComponent(nComp);
      nComp->Initialize();
    }
    else
    {

    }

    obj->tweakListComponents = false;
    obj->UpdateTweakBar();
  }


  /// <summary>
  /// Closes the object tweak bar.
  /// </summary>
  /// <param name="clientData">The client data.</param>
  static void CloseObjectTweak(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);
    ATWEAK->RemoveBar(ATWEAK->GetBar(obj->tweakHandle));
    obj->tweakHandle = Handle::null;
  }

  /// <summary>
  /// Updates the tweak bar.
  /// </summary>
  void GameObject::UpdateTweakBar()
  {
    if (tweakHandle == Handle::null)
      return;

    AntTweak::TBar* objectBar = ATWEAK->GetBar(tweakHandle);

    ErrorIf(objectBar == nullptr, "Object Tweaking", "Object tweak handle was invalid! ObjectName: '%s'", name.c_str());

    // Clear the bar of everything on it
    objectBar->Reset();

    // Get the object name
    std::string objName(name);
    objName += " [GUID: ";
    objName += std::to_string(guid) + "]";

    objectBar->SetLabel(objName.c_str());

    // Get a const reference to the object members for speed
    const std::vector<Member>& objMembers = GET_TYPE(GameObject)->GetMembers();

    for (size_t i=0; i < objMembers.size(); ++i)
    {
      // Create a variable of the object member and then tweak it
      //objectBar->DefineGroup("Properties");

      const Member* member = &objMembers[i];

      if (member->CanTweak())
      {
        if (member->TweakLabel())
          objectBar->DefineLabel(member->TweakLabel());

        // Since this is an object member variable we are tweaking we have to use the generic tweak
        objectBar->AddGenericVarRW(member->Name(), member->Type()->GetAType(), member, this);
      }
    }

    objectBar->DefineLabel("Update Archetype");
    objectBar->DefineHelpMessage("Saves this object as an archetype to a file.");
    objectBar->AddButton("ArchetypeUpdate", UpdateArchetype, this->tweakLookup);

    if (tweakListComponents)
      objectBar->DefineLabel("Hide List");
    else
      objectBar->DefineLabel("Add Component");
    objectBar->DefineHelpMessage("Displays a list of components to add.");
    objectBar->AddButton("AddComponent", ShowAddComponentList, this->tweakLookup);

    objectBar->AddSeparator("PropertySeperator");

    if (tweakListComponents)
    {
      objectBar->DefineLabel("Choose a Component");
      objectBar->AddButton("ChooseComponent", nullptr, nullptr);

      std::string bName;

      for (size_t i = 0; i < ecountComponents - 1; ++i)
      {
        if (!HasComponent(i))
        {
          bName = "Add";
          bName += EnumComponent.m_literals[i];

          objectBar->DefineLabel(EnumComponent.m_literals[i].c_str());
          objectBar->AddButton(bName.c_str(), TweakAddComponent,(*tweakCCompCallbacks)[i]);
        }
      }

      objectBar->AddSeparator("NewComponentSeperator");
    }

    objectBar->AddButton("Components", nullptr, nullptr);

    // Iterate through components
    // Tweak Each component
    for (size_t i=0; i < ecountComponents - 1; ++i)
    {
      if (HasComponent(i))
      {
        // Establish which component we are tweaking
        std::string compType = EnumComponent.m_literals[i];
        Variable var(GET_STR_TYPE(compType), GetComponent(i));

        // Prepare the AntTweak bar definition for groups
        compType = std::string(" group=") + compType;

        // Set the define and tweak the component
        objectBar->AddDefinition(compType.c_str(), true);
        var.Tweak(objectBar, nullptr, nullptr);
        objectBar->ClearDefinitions();
      }
    }

    objectBar->AddButton("Close", CloseObjectTweak, tweakLookup);

  }


  /// <summary>
  /// Tweaks the object.
  /// </summary>
  void GameObject::TweakObject()
  {
    if (tweakHandle == Handle::null)
      CustomTweak(nullptr, Variable(this), nullptr, nullptr);
    else
      UpdateTweakBar();
  }



  /// <summary>
  /// Function to get a component from lua.
  /// We have to use this function because lua is very sensitive about the type of objects that it gets
  /// It would make sense for this function to return a GameComponent* but lua doesn't know about
  /// Inheritance in C++, so it would literally assume we are sending it a GameComponent. Because of this
  /// We have to manually send it the component it wants properly typed. Therefore this function returns void.
  /// Using our introspection system and our variable class we are capable of doing this.
  /// </summary>
  /// <param name="type">The type of component</param>
  void GameObject::LuaGetComponent(size_t type)
  {

    // Quickly check to see if the component exists
    if (!HasComponent(EComponent(type)))
    {
      // If the component does not exist we manually send a FALSE value to lua

      bool b = false;
      Variable var(GET_TYPE(bool), &b);
      var.ToLua(ENGINE->Lua());

      return;
    }
    // If the component exists then we create a Variable representing the component
    Variable var(GET_STR_TYPE(EnumComponent.m_literals[type].c_str()), space->GetHandles().Get(m_components[type]));
    // And then manually pass it to lua
    Lua::GenericToLua(ENGINE->Lua(), var);

    return;
  }

  bool GameObject::LuaHasComponent(size_t type)
  {
    if (!HasComponent(EComponent(type)))
      return false;
    else
      return true;
  }

};