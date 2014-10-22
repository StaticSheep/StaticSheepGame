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
    Default constructor for sound events. Just sets it to play once and not
    currently playing.
*/
/*****************************************************************************/
SoundEvent::SoundEvent() : _mode(PLAY_ONCE), _playing(false)
{

}

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
SoundEvent::SoundEvent(SOUND::System *system, std::string &name) : 
                                                              _mode(PLAY_ONCE), 
                                                              _playing(false)
{
  // get the ID and check it
  ErrorCheck(system->lookupID(name.c_str(), &_id) );

  // get the event description
  ErrorCheck(system->getEvent(name.c_str(), &_description) );

  _pitch = 1.0f;
}


/*****************************************************************************/
/*!
  \brief
    Public method for playing a sound event
  
  \param mode
    How the sound should be played. Once, looped, or streamed
*/
/*****************************************************************************/
SOUND::EventInstance* SoundEvent::Play(PlayMode mode)
{
  // set the mode 
  _mode = mode;

  // call the correct private method for playing
  switch(mode)
  {
  case PLAY_ONCE:
    _PlayOnce();
    break;

  case PLAY_LOOP:
    _PlayLoop();
    break;

  case PLAY_STREAM:
    _PlayStream();
    break;

  default:
    _PlayOnce();
    break;
  }

  return _instance;
}

/*****************************************************************************/
/*!
  \brief
    Public method for stopping a sound event
  
  \param mode
    How the sound should fadeout... currently using built in FMOD fadeout.
*/
/*****************************************************************************/
void SoundEvent::Stop(FadeOut mode)
{
  // using fmod fadeout currently... write different fade outs later
  FMOD_STUDIO_STOP_MODE fadeout = mode ? FMOD_STUDIO_STOP_ALLOWFADEOUT : 
                                         FMOD_STUDIO_STOP_IMMEDIATE;

  // tell fmod to stop the instance with the fadeout mode
  ErrorCheck(_instance->stop(fadeout));

  _playing = 0;

  return;
}

void SoundEvent::Pause(void)
{
  bool paused;

  // check if we are paused...
  ErrorCheck(_instance->getPaused(&paused));

  // and set it to the opposite
  ErrorCheck(_instance->setPaused(!paused));


  return;
}

/*****************************************************************************/
/*!
  \brief
    Getter for getting the current mode the event is in.
*/
/*****************************************************************************/
PlayMode SoundEvent::GetMode()
{
  return _mode;
}

void SoundEvent::GetChannelGroup(FMOD::ChannelGroup* group)
{
  if(!ErrorCheck(_instance->getChannelGroup(&group)))
  {
    return;
  }

  return;
}

/*****************************************************************************/
/*!
  \brief
    Getter for checking the the event is currently playing
*/
/*****************************************************************************/
bool SoundEvent::PlayState()
{
  return _playing;
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound only once
*/
/*****************************************************************************/
SOUND::EventInstance* SoundEvent::_PlayOnce()
{
  // create the sound event
  ErrorCheck( _description->createInstance(&_instance) );

  // play it once...
  ErrorCheck( _instance->start() );

  // then release it
  //ErrorCheck( _instance->release() );

  // no longer playing
  _playing = false;

  return _instance;
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound in a loop
*/
/*****************************************************************************/
SOUND::EventInstance* SoundEvent::_PlayLoop()
{
  // if we are already playing this, then just return
  if(_playing)
    return _instance;

  // create the sound event
  ErrorCheck( _description->createInstance(&_instance) );

  // play it once...
  ErrorCheck( _instance->start() );

  // set playing to true
  _playing = true;

  return _instance;
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound in a stream
*/
/*****************************************************************************/
SOUND::EventInstance* SoundEvent::_PlayStream()
{
  // if we are not playing... then create the sound instance
  if(!_playing)
  {
    ErrorCheck( _description->createInstance(&_instance) );
    // start it
    ErrorCheck( _instance->start() );

  }

  // and set playing to true
  _playing = true;

  return _instance;
}

void SoundEvent::SetPitch(float newPitch)
{
  _pitch = newPitch;
  _instance->setPitch(_pitch);
}
