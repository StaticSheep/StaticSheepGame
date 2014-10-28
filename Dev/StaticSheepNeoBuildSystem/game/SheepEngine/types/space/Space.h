/*****************************************************************
Filename: Space.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once
#include "memory/ObjectAllocator.h"
#include "engine/handlemanager/HandleManager.h"
#include "components/base/Component.h"

namespace Framework
{
  class GameComponent;
  class GameObject;

	class GameSpace
	{
  public:
    GameSpace();
    ~GameSpace();

    void SetPaused(bool paused);
    bool Paused();

    void SetHidden(bool hidden);
    bool Hidden();

    /*template <typename T>
    void SyncHandles(ObjectAllocator& m_allocator, bool force = false);*/

    // Creates a component of a specific type
    GameComponent* CreateComponent(EComponent type);

    // Creates a component of a specific type
    GameComponent* CreateComponent(size_t type);

    // Creates an empty object
    GameObject* CreateEmptyObject();

    // Marks every single object in the space for deletion
    void Clear(void);
    
    // Tweaks the space
    void Tweak(void);

    void RemoveComponent(GameComponent* comp);
    void RemoveComponent(Handle handle);

    GameObject* GetGameObject(Handle handle);

    // Used for updating the tweak bar
    void UpdateTweakBar(void);

    // Serialization routine
    static void Serialize(File& file, Variable var);

    // Deserialization routine, var must be an empty object
    static void Deserialize(File& file, Variable var);

    // AntTweak bar creator
    static void CustomTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);

    // Gets the object allocator for a type of components
    ObjectAllocator* GetComponents(EComponent type);

    // Gets a specific component from an object
    GameComponent* GetComponent(EComponent type, Handle owner);

    HandleManager& GetHandles();

    HookManager hooks;

    const std::string GetName() const;

    // Marks a space to be destroyed
    void Destroy(void);
    

    // If a space is invalid at the end of a frame it is deleted
    bool m_valid;

    GameObject* CreateObjectFromArchetype(const char* name);

    GameSpace* CopyGameSpace(const char* new_name);
    
    // Handle used by AntTweak bar for keeping track of the game space tweak bar
    Handle tweakHandle;

    // Physics Space to simulate this game space's physics
	  void* m_pSpace;
    
    // The game space is being edited
    bool m_edit;

    unsigned int GUID() { return m_spaceGUID; }

    // Filename of the space [DO NOT WRITE TO]
    std::string m_fileName;


  private:
    // The collection of all game objects in this game space
    ObjectAllocator m_objects;

    // The collection of all components in this game space
    ObjectAllocator m_components[ecountComponents];

    // The handle manager for this game space
    HandleManager m_handles;

    // Name of the space
    std::string m_name;

    // Global Unique ID counter
    unsigned int m_guid = 0;

    unsigned int m_spaceGUID;
    static unsigned int maxGuid;

    bool m_shuttingDown;

    bool m_paused;
    bool m_hidden;

    void RemoveGameObject(GameObject* object);

    // Cleans up a space of any (to be) removed game objects.
    // Called at the end of the GameLogic update
    void Cleanup(void);

    struct SerializerData
    {
      std::vector<std::string>* instanceData;
      bool includeGeneric;
      bool saveAllData;
      bool standalone;
    };

    friend class Engine;
    friend class Factory;
    friend class GameLogic;
	};

}