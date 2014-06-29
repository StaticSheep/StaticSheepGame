/*****************************************************************
Filename: Archetype.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  const Archetype Archetype::null;

  Archetype::Archetype(void)
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
      m_components[i] = nullptr;

    name = "";
    archetype = "";
  }

  Archetype::~Archetype(void)
  {
    // Delete any components which we had allocated
    for (unsigned int i = 0; i < ecountComponents; ++i)
      if (m_components[i] != nullptr)
        delete m_components[i];
  }

  void Archetype::CopyObject(GameObject* obj)
  {
    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      if (obj->HasComponent(EComponent(i)))
      {
        // We do not have that component so lets create it quickly
        if (m_components[i] == nullptr)
          m_components[i] = FACTORY->m_componentCreators[i]->Allocate();

        // Get the type info relating to this component
        const TypeInfo *typeInfo = FACTORY->GetComponentType( EComponent(i) );

        // Iterate through all members in the component that are registered to be serialized
        for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
        {
          // Get the actual member
          const Member member = typeInfo->GetMembers()[m];
          // Copy over the member data from the object into the archetype
          memcpy((char*)m_components[i] + member.Offset(), (char*)obj->GetComponent(i) + member.Offset(), member.Type()->Size());
        }

        // Copy over everything from the object's component into the archetype. This could be dangerous
        //memcpy(m_components[i], obj->GetComponent(i), GET_STR_TYPE((GET_ENUM(Component)->m_literals[i]))->Size());
      }
      else
      {
        // The Object does not have the component, lets check to see if for some reason we do
        if (m_components[i] != nullptr)
          delete m_components[i]; // Delete the component

        // Make sure that it is a nullptr
        m_components[i] = nullptr;
      }
    } // End component iteration

    // Copy over the name and archetype of the object
    name = obj->name;
    archetype = obj->archetype;
  }

  const GameComponent* Archetype::GetComponent(unsigned int type) const
  {
    return m_components[type];
  }

  bool Archetype::HasComponent(EComponent type) const
  {
    return m_components[type] != nullptr;
  }

  void Archetype::SerializeDifferences(GameObject* obj, File& file) const
  {
    std::string instance;

    for (unsigned int j = 0; j < ecountComponents; ++j)
    {
      // Check to ensure both objects have the component, safety
      if (!obj->HasComponent(EComponent(j)) || !HasComponent(EComponent(j)))
        continue;

      GameComponent* lComp = obj->GetComponent(j);
      const GameComponent* rComp = GetComponent(j);

      // The components are the same, continue on
      if (*lComp == *rComp)
        continue;

      const TypeInfo* CType = GET_STR_TYPE(GET_ENUM(Component)->m_literals[j].c_str());

      // Prepare the string instance with the component name
      instance = CType->Name();
      instance += ":";

      // Iterate through each member of the component
      for (unsigned int m = 0; m < CType->GetMembers().size(); ++m)
      {
        // Get the member we are currently inspecting
        const Member member = CType->GetMembers()[m];

        // Compare the memory in the archetype and the object, if they are the same continue to the next member
        if (!memcmp((char*)GetComponent(j) + member.Offset(), (char*)obj->GetComponent(j) + member.Offset(), member.Type()->Size()))
          continue;

        // The data was different so we are going to print the different member now
        file.Write("  %s%s ", instance.c_str(), member.Name()); // Write the component and member

        // Create a variable from the member
        Variable var(member.Type(), (char*)obj->GetComponent(j) + member.Offset());

        // Write to the file
        var.Serialize(file);
      }

    } // End component loop
  }


}