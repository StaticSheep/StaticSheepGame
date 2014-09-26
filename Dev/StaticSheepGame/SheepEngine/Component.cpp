/*****************************************************************
Filename: Component.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

namespace Framework
{
  bool GameComponent::operator==(const GameComponent& rhs) const
  {
    const TypeInfo* lTypeData = FACTORY->GetComponentType(this->typeID);
    const TypeInfo* rTypeData = FACTORY->GetComponentType(rhs.typeID);

    if (lTypeData != rTypeData)
      return false;

    Member curMem;
    for (unsigned int i = 0; i < lTypeData->GetMembers().size(); ++i)
    {
      curMem = lTypeData->GetMembers()[i];
      // I hate adding spaces between ( )
      const TypeInfo* memberType = curMem.Type();

      if (memberType == GET_TYPE(std::string))
      {
        std::string& lhss = Variable(memberType, (char*)this + curMem.Offset()).GetValue<std::string>();
        std::string& rhss = Variable(memberType, (char*)&rhs + curMem.Offset()).GetValue<std::string>();

        if (lhss == rhss)
          continue;
        else
          return false;
      }

      if ( memcmp( (char*)this + curMem.Offset(), (char*)&rhs + curMem.Offset(), curMem.Type()->Size() ) )
        return false;
    }

    return true;
  }

  

  bool GameComponent::operator!=(const GameComponent& rhs) const
  {
    return !operator==(rhs);
  }

  GameComponent& GameComponent::operator=(const GameComponent& rhs)
  {
    const TypeInfo* lTypeData = FACTORY->GetComponentType(this->typeID);
    const TypeInfo* rTypeData = FACTORY->GetComponentType(rhs.typeID);

    if (lTypeData != rTypeData)
      return *this;

    Member curMem;
    for (unsigned int i = 0; i < lTypeData->GetMembers().size(); ++i)
    {
      curMem = lTypeData->GetMembers()[i];
      memcpy((char*)this + curMem.Offset(), (char*)&rhs + curMem.Offset(), curMem.Type()->Size());
    }

    return *this;
  }


  GameObject* GameComponent::GetOwner()
  {
    return space->GetHandles().GetAs<GameObject>(owner);
  }


}