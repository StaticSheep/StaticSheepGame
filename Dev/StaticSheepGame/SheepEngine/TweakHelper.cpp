
#include "TweakHelper.h"
#include "AntTweakModule.h"

namespace Framework
{
  namespace AntTweak
  {


    void DefaultTweak(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
    {
      const TypeInfo* typeData = var.GetTypeInfo();
      const std::vector<Member> typeMembers = typeData->GetMembers();

      // If it's not POD, we need to iterate through its members
      if (!typeData->IsPOD())
        for(size_t i=0; i < typeMembers.size(); ++i)
        {
          Member m = typeMembers[i];
          // Some members don't get tweaked, so we just skip over them
          if (m.CanTweak())
          {
            // Setup a variable for the member and send it to be tweaked
            Variable mVar(m.Type(), (char*)var.GetData() + m.Offset());
            mVar.Tweak(bar, m.Name(), m.TweakLabel());
          }
        }
      else
      {
        if (label)
          bar->DefineLabel(label);
        bar->AddVarRW(tempLabel, TBar::TranslateType(var.GetTypeInfo()->GetAType()), var.GetData());
      }
    }


    void TweakString(AntTweak::TBar* bar, Variable& var, const char* tempLabel, const char* label)
    {
      if (label)
        bar->DefineLabel(label);
      bar->AddVarRW(tempLabel, AntTweak::TW_TYPE_STDSTRING, var.GetData());
    }


  }
}