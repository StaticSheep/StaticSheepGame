/*****************************************************************
Filename: ComponentFactory.h
*****************************************************************/

#ifndef COMPCREATOR
#define COMPCREATOR

#include "Utilities.h"
#include "Space.h"

namespace Framework
{

  // Forward Declaration
  class GameComponent;

  struct ComponentCreator
  {
    // Creator Constructor

    // Type of component this constructor creates
    EComponent typeID;

    // Creation function, abstract

    // Simply creates a new component
	  
	  // Removal function, abstract

	  // The size in bytes of the component
	  unsigned int m_size;
  };

  // template version of the Creator, used to have a copy
  // Of the creator for every single type of component
  template<typename T>
  struct ComponentCreatorType : public ComponentCreator

  };


};

//#define RegisterComponent(type) Core->AddComponentCreator(Hash(#type), new ComponentCreatorType<type>(Hash(#type)))

#endif
