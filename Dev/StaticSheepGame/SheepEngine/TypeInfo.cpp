/*****************************************************************
Filename: TypeInfo.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  const TypeInfo *Member::Type(void) const
  {
    return m_typeInfo;
  }

  unsigned Member::Offset(void) const
  {
    return m_offset;
  }

  const char *Member::Name(void) const
  {
    return m_name;
  }

  TypeInfo::TypeInfo()
    : m_serialize(NULL)
    , m_deserialize(NULL)
  {
  }

  void TypeInfo::Init(const char *name, unsigned size)
  {
    std::string clean = "";
    std::string theName = name;

    for(unsigned i = 0; i < theName.length(); ++i)
      if (theName[i] != ' ')
        clean += theName[i];

    m_name = clean;
    m_size = size;
  }

  void TypeInfo::AddMember(const TypeInfo *typeInfo, const char *name, unsigned offset)
  {
    Member mem;
    mem.m_name = name;
    mem.m_offset = offset;
    mem.m_typeInfo = typeInfo;
    m_members.push_back(mem);
  }

  const Member* TypeInfo::GetMember(const char* memberName) const
  {
    for(unsigned i = 0; i < m_members.size(); ++i)
    {
      if (strcmp(m_members[i].m_name, memberName) == 0)
        return &m_members[i];
    }

    return NULL;
  }

  void TypeInfo::SetSerializer(SerializeCB cb)
  {
    m_serialize = cb;
  }

  void TypeInfo::SetDeserializer(DeserializeCB cb)
  {
    m_deserialize = cb;
  }

  void TypeInfo::Serialize(File& file, Variable var) const
  {
    if (m_serialize)
      m_serialize(file, var);
    else
      Serializer::Get()->Serialize(file, var);
  }

  void TypeInfo::Deserialize(File& file, Variable var) const
  {
    if (m_deserialize)
      m_deserialize(file, var);
    else
      Serializer::Get()->Deserialize(file, var);
  }

  unsigned TypeInfo::Size(void) const
  {
    return m_size;
  }

  const char* TypeInfo::Name(void) const
  {
    return m_name.c_str();
  }
}