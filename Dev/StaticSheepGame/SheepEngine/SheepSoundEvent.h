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

class SoundEvent
{
public:

  // constructors
  SoundEvent();
  SoundEvent(SOUND::System *system, std::string &name);

  // public methods
  SOUND::EventInstance* Play(PlayMode mode);
  void Stop(FadeOut mode);

  // getters
  PlayMode GetMode(void);
  bool PlayState(void);

private:

  // private methods
  SOUND::EventInstance* _PlayOnce(void);
  SOUND::EventInstance* _PlayLoop(void);
  SOUND::EventInstance* _PlayStream(void);

  // private members
  PlayMode _mode; 
  bool _playing;
  SOUND::ID _id;
  SOUND::EventDescription *_description;
  SOUND::EventInstance *_instance;

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

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////