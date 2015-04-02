/*****************************************************************
Filename: SheepSoundEvent.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#ifndef SHEEP_SOUND_H
#define SHEEP_SOUND_H

#include "SheepFMOD.h"

enum PlayMode
{
  PLAY_ONCE = 0,
  PLAY_LOOP,
};

enum FadeOut
{
  INSTANT = 0,
  FADE_SLOW,
  FADE_FAST,
};

// Holds all data for how to play a particular sound.
struct SoundInstance
{
  // Internal, do not set. Differentiates between Studio and low level sounds
  int type;

  // Once, Looped
  PlayMode mode;

  float volume;
  float pitch;

  // Untested, do not use yet
  float* parameters = nullptr;

  // Used with parameters, does nothing now
  int size = 0;

  // The FMOD channel this instance is playing in. Do not set.
  int channel;

  // If the sound is currently active
  bool active;
  bool isMusic;
  
  // Not implemented. Do not use.
  void SetParameters(int size, ...);
  
  union
  {
    // For events from banks.
    SOUND::EventInstance* eventInstance;

    // For objects from sound files.
    FMOD::Sound* soundInstance;
  };
  
  SoundInstance()
  {
    type = 0; mode = PLAY_ONCE; volume = 1.0f; pitch = 1.0f; active = false; isMusic = false; eventInstance = nullptr; soundInstance = nullptr;
  };
  SoundInstance(float _volume)
  {type = 0; mode = PLAY_ONCE; volume = _volume; pitch = 1.0f;active = false; isMusic = false;};
  
};

// pure virtual class for playing sounds
class Sound
{
public:
  
  Sound() : isMusic(false) {};
  
  virtual bool Play(SoundInstance* instance) = 0;
  virtual std::string GetName(void) = 0;
  virtual void UpdateVolume(float volume, bool isMusic){};
  
  bool isMusic;
  static float musicVolume;
  static float sfxVolume;

private:
  
  // say no to copy and assignment constructors
  Sound(const Sound&);
  Sound& operator=(const Sound&);
  
};

// Sound event for playing events from FMOD Studio. High level API
class SoundEvent : public Sound
{
public:

  // non-default constructor, gets the event and loads it
  SoundEvent(SOUND::System* system, std::string& name);

  // no dynamic allocation, don't need to clear anything
  ~SoundEvent(){};

  // virtual function for playing the event
  bool Play(SoundInstance* instance);

  void UpdateVolume(float volume, bool isMusic);

  // getter for getting the name of the event
  std::string GetName(){return name;};
  
private:

  // say no to copy constructor and assignment
  SoundEvent(const SoundEvent&);
  SoundEvent& operator=(const SoundEvent&);

  bool _PlayOnce(SoundInstance*);
  bool _PlayLoop(SoundInstance*);

  
  
  // description of the event from the bank files
  SOUND::EventDescription* description;

  // the id of the event
  SOUND::ID id;

  // the name of the event
  std::string name;
  

};

// Sound object for playing sounds from files. Low level FMOD API
class SoundFile : public Sound
{
public:
  
  SoundFile(FMOD::System* system, const std::string& name, FMOD::ChannelGroup* musicChannel, FMOD::ChannelGroup* sfxChannel, bool stream);

  // Plays the sound with the low level API
  bool Play(SoundInstance* instance);
  std::string GetName(){return name;};

  void UpdateVolume(float volume, bool isMusic);
  
  FMOD::Channel* GetChannel(void){return channel;};

private:
  
  SoundFile(const SoundFile&);
  SoundFile& operator=(const SoundFile&);
  
  bool _PlayOnce(SoundInstance*);
  bool _PlayLoop(SoundInstance*);

  FMOD::Sound* sound;
  std::string name;
  
  FMOD::Channel* channel;

  // The pointer to the low level FMOD system
  static FMOD::System* _system;
  static FMOD::ChannelGroup* musicChannelGroup;
  static FMOD::ChannelGroup* sfxChannelGroup;
  
};

namespace SoundUtility
{
  // enum for defining where to look for files
  enum SourceType
  {
    TYPE_AUDIO,
    TYPE_GUIDs
  };

  // function for grabbing the correct directory
  std::string SourcePath(const std::string file, SourceType type);
}

#endif
