/*****************************************************************
Filename: Archetype.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "Macros.h"
#include "Object.h"
#include "TypeInfo.h"

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

  GameComponent* Archetype::GetComponent(unsigned int type)
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

  GameObject* Archetype::CreateObject(GameSpace* space) const
  {
    GameObject* obj = space->CreateEmptyObject();

    // Set the name and archetype
    obj->name = name;
    obj->archetype = archetype;

    for (unsigned int i = 0; i < ecountComponents; ++i)
    {
      if (HasComponent(EComponent(i)))
      {
        // Create the component that is needed
        GameComponent* comp = space->CreateComponent(EComponent(i));

        // Add the component to the object
        obj->AddComponent(comp);

        // Get the type info relating to this component
        const TypeInfo *typeInfo = FACTORY->GetComponentType( EComponent(i) );

        // Iterate through all members in the component that are registered to be serialized
        for (unsigned int m = 0; m < typeInfo->GetMembers().size(); ++m)
        {
          // Get the actual member
          const Member member = typeInfo->GetMembers()[m];
          // Copy over the member data from the archetype into the object
          memcpy((char*)obj->GetComponent(i) + member.Offset(), (char*)m_components[i] + member.Offset(), member.Type()->Size());
        }

      }
    } // End component iteration

    // We added all components, now initialize the object
    obj->Initialize();

    return obj; // return the object
  }

  void Archetype::Serialize(File& file, Variable var)
  {
    Archetype* o = (Archetype*)var.GetData();
    const TypeInfo* info = GET_TYPE(Archetype);

    Serializer *s = Serializer::Get( );
    int& pad = s->GetPadLevel( );

    s->Padding( file, pad );
    file.Write( "GameObject\n" );
    s->Padding( file, pad++ );
    file.Write( "{\n" );

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

    // Now we are going to iterate through every component and serialize them
    for(unsigned i = 0; i < ecountComponents; ++i)
    {
      // Find the component type
      EComponent type = (EComponent)i;
      if (o->HasComponent( type ))
      {
        // If the object has the specified component, then get the type info
        const TypeInfo *typeInfo = FACTORY->GetComponentType( type );
        // Create a variable from the component
        const Variable v( typeInfo, o->GetComponent( type ) );
        // Serialize the component into the file
        v.Serialize( file );

        // If we aren't quite yet at the end of all the components, make a new line
        // And pad it for any future components
        if(i != ecountComponents - 1)
        {
          file.Write( "\n" );
          s->Padding( file, pad );
        }
      }
    }
    // We are all done so close the bracket
    s->Padding( file, --pad );
    file.Write( "}\n" );


  }

  void Archetype::Deserialize(File& file, Variable var)
  {
    Archetype& arch = var.GetValue<Archetype>();
    // Create a virtual game space
    GameSpace* space = new GameSpace();
    // Create an empty object in the virtual game space
    GameObject* obj = space->CreateEmptyObject();
    var = *obj; // Set the object as a variable

    // Deserialize the file into the object
    GET_TYPE(GameObject)->Deserialize(file, var);

    // Copy the object we read
    arch.CopyObject(obj);

    delete space;
  }

  Archetype& Archetype::operator=(const Archetype& rhs)
  {
    name = rhs.name;
    archetype = rhs.archetype;

    for (unsigned int i = 0; i < ecountComponents; ++i)
      if (rhs.HasComponent(EComponent(i)))
      {
        m_components[i] = FACTORY->m_componentCreators[i]->Allocate();
        *(m_components[i]) = *(rhs.m_components[i]);
      }

     return *this;
  }


}