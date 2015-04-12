/*****************************************************************
Filename: SheepSoundEvent.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "SheepSoundEvent.h"
#include <iostream>
#include <unordered_map>

// initialize static pointer for SoundFile class
FMOD::System* SoundFile::_system = 0;

static int lastErr;

// static member initialization
float Sound::sfxVolume = 1.0f;
float Sound::musicVolume = 1.0f;

FMOD::ChannelGroup* SoundFile::musicChannelGroup = nullptr;
FMOD::ChannelGroup* SoundFile::sfxChannelGroup = nullptr;

// If the result of an FMOD function was not OK, TraceLog it
bool ErrorCheck(FMOD_RESULT result)
{
  if (result != FMOD_OK)
  {
    // prevents spamming the same error every frame, not useful to see that
    if(lastErr == result)
      return true;

    lastErr = result;

    Framework::TRACELOG->Log(Framework::TraceLevel::ERR, "FMOD error! (%d)  %s", result, FMOD_ErrorString(result));
    return true;
  }
  else
    return false;

}

namespace SoundUtility
{
  // helper function to get the sourcepath of the content directory
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
}

// Constructs a sound event, throws errors in the tracelog if something bad happens
SoundEvent::SoundEvent(SOUND::System *system, std::string &name)
{
  // get the ID and check it
  ErrorCheck(system->lookupID(name.c_str(), &id) );

  // get the event description
  ErrorCheck(system->getEvent(name.c_str(), &description) );
}

// Creates an instance of a sound, plays it, and stores the pointer in instance
bool SoundEvent::Play(SoundInstance* instance)
{
  int mode = instance->mode;
  instance->type = 0;

  // call the correct private method for playing
  switch(mode)
  {
  case PLAY_ONCE : return _PlayOnce(instance);

  case PLAY_LOOP : return _PlayLoop(instance);

  default : return _PlayOnce(instance);
  }
}

// Tells a sound to play once, and releases it. Use for short SFX
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

// Creates a sound and tells it to loop. Use for Music. Keeps track of the instance*
bool SoundEvent::_PlayLoop(SoundInstance* instance)
{
  int check = 0;
  int paramSize;

  isMusic = true;

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

  ErrorCheck( instance->eventInstance->setCallback(mycallback) );

  check +=    ErrorCheck( instance->eventInstance->start() );

  if(check)
    return false;
  return true;
}

void SoundEvent::UpdateVolume(float volume, bool isMusic)
{
  if(!isMusic)
    sfxVolume = volume;
  else
  {
    musicVolume = volume;

    SOUND::EventInstance** musicInstance = 0;
    int instances = 0;

    description->getInstanceList(musicInstance, 1, &instances);

    musicInstance[0]->setVolume(musicVolume);
  }
}

// Creates a sound object from a sound file.
SoundFile::SoundFile(FMOD::System* system, const std::string& name, FMOD::ChannelGroup* musicChannel, FMOD::ChannelGroup* sfxChannel, bool stream)
{
  // if the file is too big, load it as streaming
  if(stream)
    ErrorCheck(system->createStream(name.c_str(), FMOD_CREATESTREAM, 0, &sound));
  else
    ErrorCheck(system->createSound(name.c_str(), FMOD_DEFAULT, 0, &sound));

  if(_system == NULL)
    _system = system;

  if(musicChannelGroup == NULL)
    musicChannelGroup = musicChannel;

  if(sfxChannelGroup == NULL)
    sfxChannelGroup =sfxChannel;
}
  
// Plays the sound object with the low level API
bool SoundFile::Play(SoundInstance* instance)
{
  int mode = instance->mode;
  instance->type = 1;

  // call the correct private method for playing
  switch(mode)
  {
  case PLAY_ONCE : return _PlayOnce(instance);

  case PLAY_LOOP : return _PlayLoop(instance);

  default : return _PlayOnce(instance);
  }
}

// Plays the sound once, and sets a callback for clean up
bool SoundFile::_PlayOnce(SoundInstance* instance)
{
  FMOD::Channel* localChannel;

  // play once and set all of the settings
  ErrorCheck(_system->playSound(sound, nullptr, true, &localChannel));
  ErrorCheck(localChannel->setChannelGroup(sfxChannelGroup));
  ErrorCheck(localChannel->setVolume(instance->volume));
  ErrorCheck(localChannel->setPitch(instance->pitch));

  // actually start up the sound
  ErrorCheck(localChannel->setPaused(false));

  // grab the channel index
  localChannel->getIndex(&instance->channel);

  // and give that channel a callback for when it stops
  ErrorCheck(localChannel->setCallback(mycallback));

  return true;
}

// Plays the sound in looping mode. Will not stop unless explicitely told to.
bool SoundFile::_PlayLoop(SoundInstance* instance)
{
  FMOD::Channel* localChannel;

  ErrorCheck(_system->playSound(sound, 0, true, &localChannel));

  if(instance->isMusic)
    ErrorCheck(localChannel->setChannelGroup(musicChannelGroup));
  else
    ErrorCheck(localChannel->setChannelGroup(sfxChannelGroup));

  ErrorCheck(localChannel->setVolume(instance->volume));
  ErrorCheck(localChannel->setPitch(instance->pitch));
  ErrorCheck(localChannel->setPaused(false));
  ErrorCheck(localChannel->setMode(FMOD_LOOP_NORMAL));
  ErrorCheck(localChannel->setUserData(instance));

  localChannel->getIndex(&instance->channel);

  ErrorCheck(localChannel->setCallback(mycallback));

  isMusic = true;

  return true;
}

void SoundFile::UpdateVolume(float volume, bool isMusic)
{
  if(!isMusic)
    sfxVolume = volume;
  else
  {
    musicVolume = volume;

    channel->setVolume(musicVolume);
  }
}
  
// Callback function specifically for sound objects. Low level api.
FMOD_RESULT F_CALLBACK mycallback(FMOD_CHANNELCONTROL *chanControl, FMOD_CHANNELCONTROL_TYPE controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void *commandData1, void *commandData2)
{
  // if the event is that the sound just ended
  if(callbackType == FMOD_CHANNELCONTROL_CALLBACK_END)
  {
    // grab the channel
    FMOD::Channel *channel = (FMOD::Channel *)chanControl;

    // tell FMOD the channel is free to use
    ErrorCheck(channel->stop());

  }
  return FMOD_OK;
}

// Callback function specifically for sound events. FMOD Studio
FMOD_RESULT F_CALLBACK mycallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters)
{
    FMOD::Studio::EventInstance *instance = (FMOD::Studio::EventInstance*)event;

    // if the callback event is that the sound just stopped
    if (type == FMOD_STUDIO_EVENT_CALLBACK_STOPPED)
    {
      int size;
      instance->getParameterCount(&size);

      for(int i = 0; i < size; ++i)
      {
        instance->setParameterValueByIndex(i, 1.0f);
      }
    }
    else if (type == FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND
        || type == FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND)
    {
        FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES* properties = (FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES *)parameters;

        // Handle programmer sound creation and destruction here
    }

    return FMOD_OK;
}