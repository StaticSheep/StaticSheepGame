/*****************************************************************************/
/*!
\file   SheepSound.cpp
\author Zakary Wilson
\par    email: z.wilson\@digipen.edu
\date   08/27/2014 <BR>

  \brief  
      This file contains the methods for the SoundEvent class, for creating
    and playing sounds.
*/
/*****************************************************************************/

#include "SheepSound.h"
#include <unordered_map>

void ErrorCheck(FMOD_RESULT result)
{
  if (result != FMOD_OK)
  {
    printf("FMOD error! (%d)  %s\n", result, FMOD_ErrorString(result));
  }
}

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
SoundEvent::SoundEvent(SOUND::System *system, std::string &name) : _mode(PLAY_ONCE), _playing(false)
{
  // get the ID and check it
  ErrorCheck(system->lookupID(name.c_str(), &_id) );

  // get the description
  ErrorCheck(system->getEvent( &_id, FMOD_STUDIO_LOAD_BEGIN_NOW, &_description) );
}


/*****************************************************************************/
/*!
  \brief
    Public method for playing a sound event
  
  \param mode
    How the sound should be played. Once, looped, or streamed
*/
/*****************************************************************************/
void SoundEvent::Play(PlayMode mode)
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

  return;
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
  FMOD_STUDIO_STOP_MODE fadeout = mode ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;

  // tell fmod to stop the instance with the fadeout mode
  ErrorCheck(_instance->stop(fadeout));

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
void SoundEvent::_PlayOnce()
{
  // create the sound event
  ErrorCheck( _description->createInstance(&_instance) );

  // play it once...
  ErrorCheck( _instance->start() );

  // then release it
  ErrorCheck( _instance->release() );

  // no longer playing
  _playing = false;

  return;
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound in a loop
*/
/*****************************************************************************/
void SoundEvent::_PlayLoop()
{
  // if we are already playing this, then just return
  if(_playing)
    return;

  // create the sound event
  ErrorCheck( _description->createInstance(&_instance) );

  // play it once...
  ErrorCheck( _instance->start() );

  // set playing to true
  _playing = true;

  return;
}

/*****************************************************************************/
/*!
  \brief
    Private method for playing the sound in a stream
*/
/*****************************************************************************/
void SoundEvent::_PlayStream()
{
  // if we are not playing... then create the sound instance
  if(!_playing)
    ErrorCheck( _description->createInstance(&_instance) );

  // start it
  ErrorCheck( _instance->start() );

  // and set playing to true
  _playing = true;

  return;
}
