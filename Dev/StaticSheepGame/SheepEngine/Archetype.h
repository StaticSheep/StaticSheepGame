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

    static const Archetype null;

  private:
    void CopyObject(GameObject*);
    GameComponent* m_components[ecountComponents];

    friend class Factory;
    friend class GameSpace;
  };

  template<typename T>
  T* Archetype::GetComponent(EComponent type) const
  {
    return static_cast<T*>(m_components[type]);
  }
}