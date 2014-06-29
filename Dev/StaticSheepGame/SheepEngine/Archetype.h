/*****************************************************************
Filename: Archetype.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Archetype
  {
  public:
    ~Archetype();
    Archetype();

    std::string name;
    std::string archetype;

    template<typename T>
    T* GetComponent(EComponent type) const;

    const GameComponent* GetComponent(unsigned int type) const;
    
    bool HasComponent(EComponent type) const;

    void SerializeDifferences(GameObject* obj, File& file) const;

    // Serialization routine
    void Serialize(File& file) const;
    // Deserialization routine, var must be an empty archetype (buffer)
    static void Deserialize(File& file, Archetype& arch);

    GameObject* CreateObject(GameSpace* space) const;

    static const Archetype null;

  private:
    void CopyObject(GameObject*);

    GameComponent* GetComponent(unsigned int type);

    GameComponent* m_components[ecountComponents];

    Archetype& operator=(const Archetype& rhs);

    friend class Factory;
    friend class GameSpace;
  };

  template<typename T>
  T* Archetype::GetComponent(EComponent type) const
  {
    return static_cast<T*>(m_components[type]);
  }
}