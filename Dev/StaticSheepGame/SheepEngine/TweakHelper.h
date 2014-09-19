
#pragma once

namespace Framework
{
  namespace AntTweak
  {
    void DefaultTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);
    
    void TweakString(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);

    class TweakGenericVar
    {
    public:
      TweakGenericVar(const Member* member)
        : genericMember(member)
      {

      }
      // Which member of the generic object is this tweak for
      // Contains the location and type of member
      const Member* genericMember;
      // What is the handle of the generic object we want to tweak?
      Handle genericHandle;
      // What space does the generic object belong inside of?
      GameSpace* genericSpace;
      // Handle of the TweakGenericVar struct
      Handle self;
    };



    // Plain old data tweaking

    //template<typename T>
    //void TweakPOD(TBar* bar, const Variable& var, const char* tempLabel, const char* label);

    //template<>
    //void TweakPOD<std::string>(TBar* bar, const Variable& var, const char* tempLabel, const char* label);
  }
}