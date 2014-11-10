/******************************************************************************
Filename: SheepSoundEvent.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#ifndef SHEEP_SOUND_H
#define SHEEP_SOUND_H

#include "SheepFMOD.h"

enum PlayMode
{
  PLAY_ONCE = 0,
  PLAY_LOOP,
  PLAY_STREAM
};

enum FadeOut
{
  INSTANT = 0,
  FADE_SLOW,
  FADE_FAST,
  BEES
};

struct SoundInstance
{
  int type;
  PlayMode mode;
  float volume;
  float pitch;
  float* parameters = nullptr;
  int size = 0;
  int channel;
  bool active;
  
  void SetParameters(int size, ...); // variadic function ftw
  
  union
  {
    SOUND::EventInstance* eventInstance;
    FMOD::Sound* soundInstance;
  };
  
  SoundInstance()
  {type = 0; mode = PLAY_ONCE; volume = 1.0f; pitch = 1.0f;active = false;};
  SoundInstance(float _volume)
  {type = 0; mode = PLAY_ONCE; volume = _volume; pitch = 1.0f;active = false;};
  
};

// pure virtual class for playing sounds
class Sound
{
public:
  
  Sound(){};
  
  virtual bool Play(SoundInstance* instance) = 0;
  virtual std::string GetName(void) = 0;
  
private:
  
  // say no to copy and assignment constructors
  Sound(const Sound&);
  Sound& operator=(const Sound&);
  
};

class SoundEvent : public Sound
{
public:

  // non-default constructor, gets the event and loads it
  SoundEvent(SOUND::System* system, std::string& name);

  // no dynamic allocation, don't need to clear anything
  ~SoundEvent(){};

  // virtual function for playing the event
  bool Play(SoundInstance* instance);

  // getter for getting the name of the event
  std::string GetName(){return name;};
  
private:

  // say no to copy constructor and assignment
  SoundEvent(const SoundEvent&);
  SoundEvent& operator=(const SoundEvent&);

  // private methods for playing things
  bool _PlayOnce(SoundInstance*);
  bool _PlayLoop(SoundInstance*);
  bool _PlayStream(SoundInstance*);
  
  // description of the event from the bank files
  SOUND::EventDescription* description;
  // the id of the event
  SOUND::ID id;

  // the name of the event
  std::string name;

};

class SoundFile : public Sound
{
public:
  
  SoundFile(FMOD::System* system, const std::string& name, bool stream);
  bool Play(SoundInstance* instance);
  std::string GetName(){return name;};
  
  FMOD::Channel* GetChannel(void){return channel;};

private:
  
  SoundFile(const SoundFile&);
  SoundFile& operator=(const SoundFile&);
  
  bool _PlayOnce(SoundInstance*);
  bool _PlayLoop(SoundInstance*);
  bool _PlayStream(SoundInstance*);

  FMOD::Sound* sound;
  std::string name;
  FMOD::Channel* channel;
  
  static FMOD::System* _system;
  
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
