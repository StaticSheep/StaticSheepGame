/*****************************************************************
Filename: TypeInfo.cpp
Project: 
Author(s): Zachary Nawar (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
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
    : m_serialize(nullptr), m_deserialize(nullptr), m_metatable(nullptr), m_fromLua(nullptr), m_toLua(nullptr) {}

  void TypeInfo::Init(const char* name, unsigned int size)
  {
    std::string clean = "";
    std::string theName = name;

    for(unsigned i = 0; i < theName.length(); ++i)
      if (theName[i] != ' ')
        clean += theName[i];

    m_name = clean;
    m_size = size;
  }

  void TypeInfo::AddMember(const TypeInfo* typeInfo, const char* name, unsigned int offset)
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

    return nullptr;
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

  const char* TypeInfo::LuaMetaTable() const
  {
    return m_metatable;
  }

  void TypeInfo::ToLua(lua_State* L, Variable var) const
  {
    if (m_toLua)
      m_toLua(L, var);
    else
      Lua::GenericToLua(L, var); // Generic lua pass function
  }

  void TypeInfo::FromLua(lua_State* L, int index, Variable* var) const
  {
    if (m_fromLua)
      m_fromLua(L, index, var);
    else
      Lua::GenericFromLua(L, index, var);
  }

  void TypeInfo::SetFromLua(FromLuaCB cb)
  {
    m_fromLua = cb;
  }

  void TypeInfo::SetToLua(ToLuaCB cb)
  {
    m_toLua = cb;
  }

  const std::vector<Member>& TypeInfo::GetMembers() const
  {
    return m_members;
  }
}