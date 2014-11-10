
#pragma once

namespace Framework
{
  namespace AntTweak
  {
    class TweakGenericVar;

    class Tweaker
    {
    public:
      static void DefaultTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);
      static void TweakString(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);
      static void DoTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label);

      static Function tweakGetCB;
      static Function tweakSetCB;
      static unsigned tweakOffset;
      static bool isTweakGeneric;
      static const Member* tweakMember;
      static Generic* tweakGeneric;
      static TweakGenericVar* currentGeneric;
    };
    

    typedef void(*SetCallback)(const void* value, void* clientData);
    typedef void(*GetCallback)(void* value, void* clientData);


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
      // Artificial memberOffset
      unsigned realOffset;
      // What is the handle of the generic object we want to tweak?
      Handle genericHandle;
      // What space does the generic object belong inside of?
      GameSpace* genericSpace;
      // Handle of the TweakGenericVar struct
      Handle self;
      // Extra Setcallback
      Function setCB;
      // Extra Getcallback
      Function getCB;
    };



    // Plain old data tweaking

    //template<typename T>
    //void TweakPOD(TBar* bar, const Variable& var, const char* tempLabel, const char* label);

    //template<>
    //void TweakPOD<std::string>(TBar* bar, const Variable& var, const char* tempLabel, const char* label);
  }
}