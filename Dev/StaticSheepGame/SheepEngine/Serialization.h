/*****************************************************************
Filename: Serialization.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

namespace Framework
{
  class Serializer
  {
  public:
    static Serializer* Get(void)
    {
      static Serializer instance;
      return &instance;
    }

    void Serialize(File& file, Variable var);
    void Deserialize(File& file, Variable var);

    void Padding(File& file, unsigned int count);

    const TypeInfo* PeekType(File& file, int stopLevel = 0);

    int& GetPadLevel(void);
    
    // We use these in case we feel like setting some special data
    // on the serializer itself that we want accessible from anywhere
    void* GetUserData(void) const;
    void SetUserData(void* data);

  private:
    int m_paddingLevel;
    void *m_userData;

    Serializer();
    ~Serializer();

    const Member* PeekMember(File& file, const std::vector<Member>& vec, int startLevel);

    friend class Factory;
  };
}