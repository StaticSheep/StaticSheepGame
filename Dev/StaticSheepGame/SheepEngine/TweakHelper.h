
#pragma once

namespace Framework
{
  namespace AntTweak
  {
    void DefaultTweak(TBar* bar, const Variable& var, const char* tempLabel, const char* label);
    

    // Plain old data tweaking

    //template<typename T>
    //void TweakPOD(TBar* bar, const Variable& var, const char* tempLabel, const char* label);

    //template<>
    //void TweakPOD<std::string>(TBar* bar, const Variable& var, const char* tempLabel, const char* label);
  }
}