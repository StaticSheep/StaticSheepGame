/*****************************************************************
Filename: Object.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "Object.h"
#include <algorithm>
#include <functional>
#include <iterator>

#include "components/lua/CLuaComponent.h"

#include "systems/anttweak/AntTweakModule.h"
#include "systems/editor/GizmoEditor.h"

namespace Framework
{

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

    std::string barID = std::to_string(obj.space->GUID()) + std::string("_") + std::to_string(obj.guid);

    // Create a new bar for this object
    AntTweak::TBar* objectBar = ATWEAK->CreateBar(barID.c_str());
    obj.tweakHandle = objectBar->self;
    obj.tweakListComponents = false;
    obj.tweakDeleteComponent = false;
    obj.tweakDeleteObject = false;
    objectBar->SetIconify(false);
    objectBar->SetSize(250, 350);
    objectBar->SetValueWidth(100);

    static int count = 0;
    static int count2 = 0;

    

    if (GIZMO_EDITOR->m_oneObjectBar)
    {
      objectBar->SetPos(15, 350);
    }
    else
    {
      objectBar->SetPos(430 + 255 * count++, 16 + count2 * 20);
      if (count > 3)
      {
        count = 0;
        ++count2;
        if (count2 > 4)
          count2 = 0;
      }
    }
    

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
  /// Generates the component list for an object.
  /// </summary>
  /// <param name="gl">The gl.</param>
  /// <param name="obj">The object.</param>
  static void GenerateComponentList(GenericLookup* gl, GameObject* obj)
  {
    if (obj->tweakCCompCallbacks == nullptr)
    {
      obj->tweakCCompCallbacks = DBG_NEW std::vector<TweakObjComp*>();

      // Load up the C++ Component callback tables
      for(size_t i=0; i < ecountComponents - 1; ++i)
      {
        std::string divlabel(EnumComponent.m_literals[i].begin(),
          EnumComponent.m_literals[i].begin() + 4);

        if (divlabel == "div_")
        {
          obj->tweakCCompCallbacks->push_back(nullptr);
          continue;
        }
          

        TweakObjComp* userdata = DBG_NEW TweakObjComp();
        userdata->gl.self = obj->self;
        userdata->gl.space = obj->space;
        userdata->compType = (EComponent)i;

        obj->tweakCCompCallbacks->push_back(userdata);
      }

      obj->tweakLuaCompCallbacks = DBG_NEW std::vector<TweakObjComp*>();

      // Load up the Lua Component callback tables
      const std::vector<std::string>& compList = ENGINE->LuaComponentList();

      for (unsigned i = 0; i < compList.size(); ++i)
      {
        TweakObjComp* userdata = DBG_NEW TweakObjComp();
        userdata->gl.self = obj->self;
        userdata->gl.space = obj->space;
        userdata->compType = eLuaComponent;
        userdata->luaCompName = compList[i];

        obj->tweakLuaCompCallbacks->push_back(userdata);
      }

    }
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

    GenerateComponentList(gl, obj);

    obj->UpdateTweakBar();
    
  }

  /// <summary>
  /// Shows the remove component list.
  /// </summary>
  /// <param name="clientData">The client data.</param>
  static void ShowRemoveComponentList(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);

    obj->tweakListComponents = !obj->tweakListComponents;
    obj->tweakDeleteComponent = !obj->tweakDeleteComponent;

    GenerateComponentList(gl, obj);

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
      nComp->owner = obj->self;
      nComp->Initialize();
    }
    else
    {
      LuaComponent* nComp =(LuaComponent*)
        oc->gl.space->CreateComponent(eLuaComponent);
      nComp->name = luaType;
      obj->AddComponent(nComp);
      nComp->owner = obj->self;
      nComp->Initialize();
    }

    

    obj->tweakListComponents = false;
    obj->UpdateTweakBar();
  }

  /// <summary>
  /// Callback for AntTweak bar removing a component on an object
  /// </summary>
  /// <param name="clientData">The client data.</param>
  static void TweakRemoveComponent(void* clientData)
  {
    TweakObjComp* oc = (TweakObjComp*)clientData;
    GameObject* obj = oc->gl.space->GetGameObject(oc->gl.self);
    EComponent cType = oc->compType;
    std::string luaType = oc->luaCompName;


    if (cType < eLuaComponent)
    {
      // Don't remove the Transform component you idiot!
      if (obj->HasComponent(cType) && cType != eTransform)
        obj->DetatchComponent(cType);
    }
    else
    {
      int luaIndex = obj->HasLuaComponent(luaType.c_str());
      if (luaIndex != -1)
      {
        // Detach Lua component
        obj->DetatchLuaComponent(luaIndex);
      }
    }

    //obj->Initialize(); //This may or may not be needed?

    obj->tweakDeleteComponent = false;
    obj->tweakListComponents = false;
    obj->UpdateTweakBar();
  }

  void GameObject::TweakUpdateArchetypes(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);

    UpdateArchetype(clientData);

    FACTORY->UpdateArchetypeObjects(obj->space, obj->archetype.c_str());
  }

  /// <summary>
  /// Closes the object tweak bar.
  /// </summary>
  /// <param name="clientData">The client data.</param>
  void GameObject::CloseObjectTweak(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);
    ATWEAK->RemoveBar(ATWEAK->GetBar(obj->tweakHandle));
    obj->tweakHandle = Handle::null;
  }

  static void DeleteObjectTweak(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;
    GameObject* obj = gl->space->GetGameObject(gl->self);

    if (obj->tweakDeleteObject)
    {
      obj->Destroy();
    }
    else
    {
      obj->tweakDeleteObject = true;
      obj->UpdateTweakBar();
    }
  }

  static void DuplicateObject(void* clientData)
  {
    GenericLookup* gl = (GenericLookup*)clientData;

    GameObject* newObj = gl->space->CreateEmptyObject();
    GameObject* obj = gl->space->GetGameObject(gl->self);

    newObj->Copy(*obj);

    newObj->Initialize();

    gl->space->UpdateTweakBar();

    newObj->TweakObject();

    
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

        if (member->TweakSetCB())
        {
          objectBar->AddGenericVarCB(member->Name(), member->Type()->GetAType(),
            member, 0, this, member->TweakSetCB(), Function());
        }
        else
        {
          // Since this is an object member variable we are tweaking
          // we have to use the generic tweak
          objectBar->AddGenericVarRW(member->Name(),
            member->Type()->GetAType(), member, 0, this);
        }
        
      }
    }

    objectBar->DefineKeyShortcut("CTRL+u");
    objectBar->DefineLabel("Save Archetype");
    objectBar->DefineHelpMessage("Saves this object as an archetype to a file.");
    objectBar->AddButton("ArchetypeUpdate", UpdateArchetype, this->tweakLookup);

    objectBar->DefineLabel("Update Other Objects");
    objectBar->DefineHelpMessage("Updates other objects with the same archetype to match this object.");
    objectBar->AddButton("ArchetypeObjectUpdate", GameObject::TweakUpdateArchetypes, this->tweakLookup);

    objectBar->DefineLabel("Duplicate Object");
    objectBar->AddButton("Duplicate", DuplicateObject, this->tweakLookup);

    objectBar->AddSeparator("ComponentAddRemove");

    if (tweakListComponents)
    {
      objectBar->DefineLabel("Hide List");
      if (tweakDeleteComponent)
      {
        objectBar->DefineKeyShortcut("CTRL+r");
        objectBar->AddButton("RemoveComponent", ShowRemoveComponentList,
          this->tweakLookup);
      }
      else
      {
        objectBar->DefineKeyShortcut("CTRL+a");
        objectBar->AddButton("AddComponent", ShowAddComponentList,
          this->tweakLookup);
      }
    }
    else
    {
      objectBar->DefineKeyShortcut("CTRL+a");
      objectBar->DefineLabel("Add Component");
      objectBar->AddButton("AddComponent", ShowAddComponentList,
        this->tweakLookup);

      objectBar->DefineKeyShortcut("CTRL+r");
      objectBar->DefineLabel("Remove Component");

      objectBar->AddButton("RemoveComponent", ShowRemoveComponentList,
        this->tweakLookup);
    }

    objectBar->AddSeparator("PropertySeperator");

    if (tweakListComponents)
    {
      
      if (tweakDeleteComponent)
      {
        // ====================== REMOVE COMPONENTS ==================== //

        objectBar->DefineLabel("Delete a component");
        objectBar->AddButton("ChooseComponent", nullptr, nullptr);

        std::string bName;

        /* ------- C++ Components ------- */
        for (size_t i = 0; i < ecountComponents - 1; ++i)
        {
          if (HasComponent(i) && i != eTransform)
          {
            bName = "Remove";
            bName += EnumComponent.m_literals[i];

            objectBar->DefineLabel(EnumComponent.m_literals[i].c_str());

            objectBar->AddButton(bName.c_str(), TweakRemoveComponent,
              (*tweakCCompCallbacks)[i]);
          }
        }

        /* ------- Lua Components ------- */
        objectBar->AddSeparator("Lua Components");

        for (unsigned i = 0; i < tweakLuaCompCallbacks->size(); ++i)
        {
          TweakObjComp* lc = (*tweakLuaCompCallbacks)[i];

          if (HasLuaComponent(lc->luaCompName.c_str()) != -1)
          {
            bName = "Remove";
            bName += lc->luaCompName;

            objectBar->DefineLabel(lc->luaCompName.c_str());

            objectBar->AddButton(bName.c_str(), TweakRemoveComponent,
              (*tweakLuaCompCallbacks)[i]);
          }

        }

        // End Delete
      }
      else
      {
        // ====================== ADD COMPONENTS ==================== //

        objectBar->DefineLabel("Add a component");
        objectBar->AddButton("ChooseComponent", nullptr, nullptr);

        std::string bName;


        /* ------- C++ Components ------- */
        for (size_t i = 0; i < ecountComponents - 1; ++i)
        {
          std::string divlabel(EnumComponent.m_literals[i].begin(),
            EnumComponent.m_literals[i].begin() + 4);

          if (divlabel == "div_")
          {
            divlabel = std::string(EnumComponent.m_literals[i].begin() + 4,
              EnumComponent.m_literals[i].end());

            objectBar->DefineLabel(divlabel.c_str());

            std::string divid = "divider";
            divid += std::to_string(i);

            objectBar->AddButton(divid.c_str(), nullptr, nullptr);
            continue;
          }

          if (!HasComponent(i))
          {
            bName = "Add";
            bName += EnumComponent.m_literals[i];

            objectBar->DefineLabel(EnumComponent.m_literals[i].c_str());

            objectBar->AddButton(bName.c_str(), TweakAddComponent,
              (*tweakCCompCallbacks)[i]);
          }
        }

        /* ------- Lua Components ------- */
        objectBar->AddSeparator("Lua Components");

        for (unsigned i = 0; i < tweakLuaCompCallbacks->size(); ++i)
        {
          TweakObjComp* lc = (*tweakLuaCompCallbacks)[i];
          
          if (HasLuaComponent(lc->luaCompName.c_str()) == -1)
          {
            bName = "Add";
            bName += lc->luaCompName;

            objectBar->DefineLabel(lc->luaCompName.c_str());

            objectBar->AddButton(bName.c_str(), TweakAddComponent,
              (*tweakLuaCompCallbacks)[i]);
          }

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
        const char* compName = EnumComponent.m_literals[i].c_str();
        Variable var(GET_STR_TYPE(compType), GetComponent(i));

        // Prepare the AntTweak bar definition for groups
        compType = std::string(" group=") + compType;

        // Set the define and tweak the component
        //objectBar->AddDefinition(compType.c_str(), true);
        var.Tweak(objectBar, compName, compName);
        //objectBar->ClearDefinitions();
      }
    }

    objectBar->AddSeparator("CloseDelete");

    
    objectBar->DefineGroup("Delete");
    if (tweakDeleteObject)
      objectBar->DefineLabel("ARE YOU SURE!????");
    else
      objectBar->DefineLabel("Delete Object");

    objectBar->DefineKeyShortcut("CTRL+d");
    objectBar->AddButton("DeleteObject", DeleteObjectTweak, tweakLookup);

    objectBar->SetGroupOpened("Delete", false);

    objectBar->DefineKeyShortcut("CTRL+x");
    objectBar->AddButton("Close", GameObject::CloseObjectTweak, tweakLookup);

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

    ObjectSelectedMessage msg(this);

    ENGINE->SystemMessage(msg);
  }

  

  void GameObject::TweakSetName(void* inname)
  {
    name = *(std::string*)inname;
    space->UpdateTweakBar();
  }

}