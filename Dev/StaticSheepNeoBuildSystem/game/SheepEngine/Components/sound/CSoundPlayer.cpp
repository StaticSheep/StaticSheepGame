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
  }

  SoundPlayer::~SoundPlayer()
  {
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
  void SoundPlayer::Play(const std::string &name, SoundInstance* instance)
  {
    if(instanceList[name].active)
      return;

    instanceList[name].inst = *instance;
    instanceList[name].active = true;
    _soundSystem->Play(name, &instanceList[name].inst);
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
    _soundSystem->Stop(&instanceList[name].inst);
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
    _soundSystem->Pause(&instanceList[name].inst, flag);
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