/*****************************************************************
Filename: SheepSoundEvent.h
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

////////////////////////////////////////////////////////////////////////////////
#ifndef SHEEP_SOUND_H
#define SHEEP_SOUND_H
////////////////////////////////////////////////////////////////////////////////

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
  
};

class Sound
{
public:
  
  Sound(){};
  //virtual ~Sound() = 0;
  
  virtual bool Play(SoundInstance* instance) = 0;
  virtual std::string GetName(void) = 0;
  
private:
  
  Sound(const Sound&);
  Sound& operator=(const Sound&);
  
};

class SoundEvent : public Sound
{
public:
  ~SoundEvent(){};
  SoundEvent(SOUND::System* system, std::string& name);
  bool Play(SoundInstance* instance);
  std::string GetName(){return name;};
  
private:
  
  SoundEvent(const SoundEvent&);
  
  SoundEvent& operator=(const SoundEvent&);

  bool _PlayOnce(SoundInstance*);
  bool _PlayLoop(SoundInstance*);
  bool _PlayStream(SoundInstance*);
  
  SOUND::EventDescription* description;
  SOUND::ID id;
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


/*

class SoundEvent
{
public:

  // constructors
  SoundEvent();
  SoundEvent(SOUND::System *system, std::string &name);

  // public methods
  SOUND::EventInstance* Play(PlayMode mode, float volume, float pitch);
  void Stop(FadeOut mode);
  void Pause();

  // getters
  PlayMode GetMode(void);

  void GetChannelGroup(FMOD::ChannelGroup* group);

  void SetPitch(float);
  void SetVolume(float);

  bool PlayState(void);

private:

  // private methods
  SOUND::EventInstance* _PlayOnce(float volume, float pitch);
  SOUND::EventInstance* _PlayLoop(float volume, float pitch);
  SOUND::EventInstance* _PlayStream(float volume, float pitch);

  // private members

  float _pitch;
  float _volume;
  PlayMode _mode; 
  bool _playing;
  SOUND::ID _id;
  SOUND::EventDescription *_description;
  SOUND::EventInstance *_instance;

};*/


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

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////