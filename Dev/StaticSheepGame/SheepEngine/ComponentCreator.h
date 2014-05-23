/*****************************************************************
Filename: ComponentFactory.h
Project: 
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef COMPCREATOR
#define COMPCREATOR

#include "Utilities.h"

namespace Framework
{

  // Forward Declaration
  class GameComponent;

  class ComponentCreator
  {
    public:
      // Creator Constructor
      ComponentCreator(size_t type) : typeID(type) {};

      // Type of component this constructor creates
      size_t typeID;

      // Creation function, abstract
      virtual GameComponent* Create() = 0;
  };

  // Templated version of the Creator, used to have a copy
  // Of the creator for every single type of component
  template<typename T>
  class ComponentCreatorType : public ComponentCreator
  {
    ComponentCreatorType(size_t type) : ComponentCreator(type) {}

    // Override the create function with the type we want to make
    virtual GameComponent* Create()
    {
      return new T();
    }
  };


};

//#define RegisterComponent(type) Core->AddComponentCreator(Hash(#type), new ComponentCreatorType<type>(Hash(#type)))

#endif