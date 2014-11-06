/******************************************************************************
Filename: SheepSoundEvent.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"
#include "SheepSoundEvent.h"
#include <iostream>
#include <unordered_map>

/*****************************************************************************/
/*!
  \brief
    FMOD error check.
  
  \param result
    Checks the result of the FMOD function to see if it was an error or not.

*/
/*****************************************************************************/
bool ErrorCheck(FMOD_RESULT result)
{
  if (result != FMOD_OK)
  {
    // need to replace with exception handling from the engine
    printf("FMOD error! (%d)  %s\n", result, FMOD_ErrorString(result));
    return 1;
  }
  else
    return 0;

}


namespace SoundUtility
{

/*****************************************************************************/
/*!
  \brief
    Utility function to figure out where to look for sound files.
  
  \param file
    Name of the file

  \param type
    Type of file, which is an enum. 

*/
/*****************************************************************************/
  std::string SourcePath(const std::string file, SourceType type)
  {
    switch(type)
    {
      // change these for the proper media paths
      case TYPE_AUDIO : return "content\\Audio\\FMOD_Banks\\" + file; 
      case TYPE_GUIDs : return "content\\Audio\\" + file;
      default : return "content\\Audio\\FMOD_Banks\\" + file;
    }
    return "";
  }
}// end namespace


/*****************************************************************************/
/*!
  \brief
    Constructor for the SoundEvent class
  
  \param system
    Pointer to the FMOD System

  \param name
    Name of the sound file
*/
/*****************************************************************************/
SoundEvent::SoundEvent(SOUND::System *system, std::string &name)
{
  // get the ID and check it
  ErrorCheck(system->lookupID(name.c_str(), &id) );

  // get the event description
  ErrorCheck(system->getEvent(name.c_str(), &description) );
}


/*****************************************************************************/
/*!
  \brief
    Public method for playing a sound event
  
  \param mode
    How the sound should be played. Once, looped, or streamed
*/
/*****************************************************************************/
bool SoundEvent::Play(SoundInstance* instance)
{
  // set the mode 
  SoundInstance newInstance;

  if(!instance)
  {
    *instance = newInstance;
  }

  int mode = instance->mode;

  // call the correct private method for playing
  switch(mode)
  {
  case PLAY_ONCE : return _PlayOnce(instance);

  case PLAY_LOOP : return _PlayLoop(instance);

  case PLAY_STREAM : return _PlayStream(instance);

  default : return _PlayOnce(instance);
  }
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound only once
*/
/*****************************************************************************/
bool SoundEvent::_PlayOnce(SoundInstance* instance)
{
  int check = 0;
  int paramSize;
  // create the sound event
  check +=    ErrorCheck( description->createInstance(&instance->eventInstance) );
  check +=    ErrorCheck( instance->eventInstance->setPitch(instance->pitch));
  check +=    ErrorCheck( instance->eventInstance->setVolume(instance->volume));

  check +=    ErrorCheck(instance->eventInstance->getParameterCount(&paramSize));

  if(paramSize)
  {
    for(int i = 0; i < paramSize; ++i)
    {
      if(i < instance->size)
        check += ErrorCheck(instance->eventInstance->setParameterValueByIndex(i, instance->parameters[i]));
      else
        check += ErrorCheck(instance->eventInstance->setParameterValueByIndex(i, 0.0f));
    }
  }

  // play it once...
  check +=    ErrorCheck( instance->eventInstance->start() );

  // then release it
  check +=    ErrorCheck( instance->eventInstance->release() );

  if(check)
    return false;
  return true;
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound in a loop
*/
/*****************************************************************************/
bool SoundEvent::_PlayLoop(SoundInstance* instance)
{
  int check = 0;
  int paramSize;

  // create the sound event
  check +=    ErrorCheck( description->createInstance(&instance->eventInstance) );
  check +=    ErrorCheck( instance->eventInstance->setPitch(instance->pitch));
  check +=    ErrorCheck( instance->eventInstance->setVolume(instance->volume));

  check +=    ErrorCheck(instance->eventInstance->getParameterCount(&paramSize));

  if(paramSize)
  {
    for(int i = 0; i < paramSize; ++i)
    {
      if(i < instance->size)
        check += ErrorCheck(instance->eventInstance->setParameterValueByIndex(i, instance->parameters[i]));
      else
        check += ErrorCheck(instance->eventInstance->setParameterValueByIndex(i, 0.0f));
    }
  }

  check +=    ErrorCheck( instance->eventInstance->start() );

  if(check)
    return false;
  return true;
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound in a stream
*/
/*****************************************************************************/
bool SoundEvent::_PlayStream(SoundInstance* instance)
{
  int check = 0;
  int paramSize;
  // create the sound event
  check +=    ErrorCheck( description->createInstance(&instance->eventInstance) );
  check +=    ErrorCheck( instance->eventInstance->setPitch(instance->pitch));
  check +=    ErrorCheck( instance->eventInstance->setVolume(instance->volume));

  check +=    ErrorCheck(instance->eventInstance->getParameterCount(&paramSize));

  if(paramSize)
  {
    for(int i = 0; i < paramSize; ++i)
    {
      if(i < instance->size)
        check += ErrorCheck(instance->eventInstance->setParameterValueByIndex(i, instance->parameters[i]));
      else
        check += ErrorCheck(instance->eventInstance->setParameterValueByIndex(i, 0.0f));
    }
  }

  // play it once...
  check +=    ErrorCheck( instance->eventInstance->start() );

  if(check)
    return false;
  return true;
}
