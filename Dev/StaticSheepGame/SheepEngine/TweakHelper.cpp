
#include "TweakHelper.h"
#include "AntTweakModule.h"

namespace Framework
{
  namespace AntTweak
  {

    bool Tweaker::isTweakGeneric = false;
    const Member* Tweaker::tweakMember = nullptr;
    Generic* Tweaker::tweakGeneric = nullptr;
    unsigned Tweaker::tweakOffset = 0;

    void Tweaker::DoTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
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
          bar->AddGenericVarCB(tempLabel, varType, tweakMember, tweakOffset, tweakGeneric, tweakMember->TweakSetCB(), tweakMember->TweakGetCB());
        else
          bar->AddGenericVarRW(tempLabel, varType, tweakMember, tweakOffset, tweakGeneric);
      }
      else
      {
        // Good ol' variable tweaking which is simple
        bar->AddVarRW(tempLabel, varType, var.GetData());
      }
    }


    void Tweaker::DefaultTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
    {
      const TypeInfo* typeData = var.GetTypeInfo();
      const std::vector<Member>& typeMembers = typeData->GetMembers();

      // If it's not POD, it must be a class or struct
      if (!typeData->IsPOD())
      {

        // If this is a component we are tweaking then we need to indicate that we are tweaking a generic object
        // and set the generic pointer to the component
        if (typeData->GetAType() == TW_TYPE_COMPONENT)
        {
          isTweakGeneric = true;
          tweakGeneric = (GameComponent*)var.GetData();
        }

        // If we are tweaking a 3D vector or a 4D vector we can use AntTweak Bar's tweaking shit
        if (typeData->GetAType() == TW_TYPE_DIR3F || typeData->GetAType() == TW_TYPE_COLOR4F)
        {
          DoTweak(bar, var, tempLabel, label);
        }

        // Keep track of the current and last offset
        //unsigned lastOffset = tweakOffset;
        unsigned currentOffset = 0;

        // Iterate through all the members in the class we are trying to tweak
        for(size_t i=0; i < typeMembers.size(); ++i)
        {
          const Member* m = &(typeMembers[i]);
          // Some members don't get tweaked, so we just skip over them
          if (m->CanTweak())
          {
            currentOffset = m->Offset();
            
            // Add the current member offset onto the tweakOffset
            tweakOffset += currentOffset;

            // Setup a variable for the member and send it to be tweaked
            Variable mVar(m->Type(), (char*)var.GetData() + currentOffset);

            // Set which member we are tweaking on the Tweaker
            tweakMember = m;

            // If the next member is POD we stick it in a group
            if (m->Type()->IsPOD())
              if (label)
                bar->DefineGroup(label);
              else
                bar->DefineGroup(tempLabel);

            mVar.Tweak(bar, m->Name(), m->TweakLabel());

            if (!m->Type()->IsPOD())
            {
              if (m->TweakLabel())
                if (label)
                  bar->SetGroupParent(m->TweakLabel(), label);
                else
                  bar->SetGroupParent(m->TweakLabel(), tempLabel);
              else
                if (label)
                  bar->SetGroupParent(m->Name(), label);
                else
                  bar->SetGroupParent(m->Name(), tempLabel);
            }

            // Subtract the current member offset from the tweakOffset
            tweakOffset -= currentOffset;
          }
        }

        // If we had an offset before, decrement it from the tweakOffset
        //if (lastOffset)
        //  tweakOffset -= lastOffset;

        // No longer tweaking a generic
        if (isTweakGeneric)
          isTweakGeneric = false;
      }
      else // We are POD data that needs to be tweaked
      {
        DoTweak(bar, var, tempLabel, label); 
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
          bar->AddGenericVarCB(tempLabel, AntTweak::TW_TYPE_STDSTRING, tweakMember, tweakOffset, tweakGeneric, tweakMember->TweakSetCB(), tweakMember->TweakGetCB());
        else
          bar->AddGenericVarRW(tempLabel, AntTweak::TW_TYPE_STDSTRING, tweakMember, tweakOffset, tweakGeneric);
        
      }
      else
      {
        bar->AddVarRW(tempLabel, AntTweak::TW_TYPE_STDSTRING, var.GetData());
      }
      
    }


  }
}