/*****************************************************************
Filename: Serialization.h
Project: 

*****************************************************************/

#pragma once

namespace Framework
{
  class Serializer
  {
  public:
    {
      static Serializer instance;
      return &instance;
    }





    
    // We use these in case we feel like setting some special data
    // on the serializer itself that we want accessible from anywhere

  private:
    int m_paddingLevel;
    void *m_userData;


    

    friend class Factory;
  };
}
