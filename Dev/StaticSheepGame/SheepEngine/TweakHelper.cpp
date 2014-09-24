
#include "TweakHelper.h"
#include "AntTweakModule.h"

namespace Framework
{
  namespace AntTweak
  {

    bool Tweaker::isTweakGeneric = false;
    const Member* Tweaker::tweakMember = nullptr;
    Generic* Tweaker::tweakGeneric = nullptr;


    void Tweaker::DefaultTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
    {
      const TypeInfo* typeData = var.GetTypeInfo();
      const std::vector<Member>& typeMembers = typeData->GetMembers();

      // If it's not POD, we need to iterate through its members
      if (!typeData->IsPOD())
      {

        // If this is a component we are tweaking then we need to indicate that we are tweaking a generic object
        // and set the generic pointer to the component
        if (typeData->GetAType() == TW_TYPE_COMPONENT)
        {
          isTweakGeneric = true;
          tweakGeneric = (GameComponent*)var.GetData();
        }

        for(size_t i=0; i < typeMembers.size(); ++i)
        {
          const Member* m = &(typeMembers[i]);
          // Some members don't get tweaked, so we just skip over them
          if (m->CanTweak())
          {
            // Setup a variable for the member and send it to be tweaked
            Variable mVar(m->Type(), (char*)var.GetData() + m->Offset());
            // Set which member we are tweaking on the Tweaker
            tweakMember = m;
            mVar.Tweak(bar, m->Name(), m->TweakLabel());
          }
        }

        // No longer tweaking a generic
        if (isTweakGeneric)
          isTweakGeneric = false;
      }
      else // We are POD data that needs to be tweaked
      {
        if (label)
          bar->DefineLabel(label);

        // Translate the type here, I actually don't think we even use this shit but whatever just in case
        engineTwType varType =  TBar::TranslateType(var.GetTypeInfo()->GetAType());

        // If we are tweaking a variable which belongs to a generic object (component or object) then we have to
        // use a special function for tweaking which requires a member* and a generic*
        if (isTweakGeneric)
        {
          // If there is a tweaking get or set callback for this type then we need to use another function
          if (tweakMember->TweakSetCB() != nullptr)
            bar->AddGenericVarCB(tempLabel, varType, tweakMember, tweakGeneric, tweakMember->TweakSetCB(), tweakMember->TweakGetCB());
          else
            bar->AddGenericVarRW(tempLabel, varType, tweakMember, tweakGeneric);
        }
        else
        {
          // Good ol' variable tweaking which is simple
          bar->AddVarRW(tempLabel, varType, var.GetData());
        }
        
      }
    }


    void Tweaker::TweakString(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
    {
      if (label)
        bar->DefineLabel(label);

      // If we are tweaking a variable which belongs to a generic object (component or object) then we have to
      // use a special function for tweaking which requires a member* and a generic*
      if (isTweakGeneric)
      {
        // If there is a tweaking get or set callback for this type then we need to use another function
        if (tweakMember->TweakSetCB() != nullptr)
          bar->AddGenericVarCB(tempLabel, AntTweak::TW_TYPE_STDSTRING, tweakMember, tweakGeneric, tweakMember->TweakSetCB(), tweakMember->TweakGetCB());
        else
          bar->AddGenericVarRW(tempLabel, AntTweak::TW_TYPE_STDSTRING, tweakMember, tweakGeneric);
        
      }
      else
      {
        bar->AddVarRW(tempLabel, AntTweak::TW_TYPE_STDSTRING, var.GetData());
      }
      
    }


  }
}