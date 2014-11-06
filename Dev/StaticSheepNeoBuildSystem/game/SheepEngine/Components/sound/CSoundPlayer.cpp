/******************************************************************************
Filename: SoundPlayer.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"
#include "CSoundPlayer.h"


namespace Framework
{

/*****************************************************************************/
/*!
  \brief
    Default constructor for the SoundPlayer component
*/
/*****************************************************************************/
  SoundPlayer::SoundPlayer()
  {
    // connect to the FMOD system
    _soundSystem = AUDIO;
    _volume = 1.0f;
    _pitch = 1.0f;

    // null out the array of pointers
    for(int i = 0; i < MAX_SOUNDS; ++i)
    {
      _instanceList[i].instance = NULL;
    }
  }

  SoundPlayer::~SoundPlayer()
  {
    for(int i = 0; i < MAX_SOUNDS; ++i)
    {
      _instanceList[i].instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
      _instanceList[i].instance->release();
      _instanceList[i].instance = NULL;
      _instanceList[i].name = "null";
    }
  }
  
/*****************************************************************************/
/*!
  \brief
    Plays an event! Creates an instance that the SoundPlayer keeps track of
    so that the sound can be stopped or paused later. 

  \param name
    Name of the event to play

  \param mode
    How to play the sound. PLAY_ONCE or PLAY_LOOP (PLAY_STREAM is pretty much
    PLAY_LOOP in disguise...)
*/
/*****************************************************************************/
  void SoundPlayer::Play(const std::string &name, PlayMode mode)
  {
    for(int i = 0; i < MAX_SOUNDS; ++i)
    {
      if(_instanceList[i].instance == NULL)
      {
        _instanceList[i].instance = _soundSystem->Play(name, mode, _volume, _pitch);
        _instanceList[i].name = name;

        break;
      }
    }
  }

/*****************************************************************************/
/*!
  \brief
    Extended functionality to the play function. Can set the volume of that
    instance.

  \param name
    Name of the event to play

  \param mode
    How to play the sound. PLAY_ONCE or PLAY_LOOP (PLAY_STREAM is pretty much
    PLAY_LOOP in disguise...)

  \param volume
    How loud to play the sound. It is a normalized value, between 0 - 1.
*/
/*****************************************************************************/  
  void SoundPlayer::PlayEx(const std::string &name, PlayMode mode, float volume)
  {
    for(int i = 0; i < MAX_SOUNDS; ++i)
    {
      if(_instanceList[i].instance == NULL)
      {
        _instanceList[i].instance = _soundSystem->Play(name, mode, volume, _pitch);
        _instanceList[i].name = name;

        break;
      }
    }
  }
  
/*****************************************************************************/
/*!
  \brief
    Stops an instance of an event that is playing.

  \param name
    Name of the event to stop

  \param mode
    How to stop the instance. Immedate or faded
*/
/*****************************************************************************/
  void SoundPlayer::Stop(std::string name, FadeOut mode)
  {
    for(int i = 0; i < MAX_SOUNDS; ++i)
    {
      if(_instanceList[i].name == name)
      {
        _instanceList[i].instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
        _instanceList[i].instance->release();
        _instanceList[i].instance = NULL;
        _instanceList[i].name = "null";
        break;
      }
    }
  }

/*****************************************************************************/
/*!
  \brief
    Pauses an event, so it can be picked back up and played again.

  \param name
    Name of the event to pause

  \param flag
    True means it will pause, false means it will unpause
*/
/*****************************************************************************/  
  void SoundPlayer::Pause(std::string name, bool flag)
  {
    for(int i = 0; i < MAX_SOUNDS; ++i)
    {
      if(_instanceList[i].name == name)
      {
        _instanceList[i].instance->setPaused(flag);
        break;
      }
    }
  }
  
/*****************************************************************************/
/*!
  \brief
    Sets the pitch of the player. Currently does now update the currently
    playing instances.

  \param pitch
    What to reset the pitch to
*/
/*****************************************************************************/
  void SoundPlayer::SetPitch(float pitch)
  {
    _pitch = pitch;
  }

}// end namespace