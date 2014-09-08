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
  void Play(PlayMode mode);
  void Stop(FadeOut mode);

  // getters
  PlayMode GetMode(void);
  bool PlayState(void);

private:

  // private methods
  void _PlayOnce(void);
  void _PlayLoop(void);
  void _PlayStream(void);

  // private members
  PlayMode _mode;
  bool _playing;
  SOUND::ID _id;
  SOUND::EventDescription *_description;
  SOUND::EventInstance *_instance;

};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////