
#include <string>
#include <unordered_map>
#include "SheepSound.h"
#include "SoundStrings.h"

class SoundManager
{
public:

  // constructor, needs to setup FMOD and load all sounds
  SoundManager(const std::string &guidfile = "GUIDs.txt");

  // destructor, needs to free all resources
  ~SoundManager();

  // public methods
  void Update();

  void Play(const std::string &event_name, PlayMode mode);
  void Stop(const std::string &event_name, FadeOut mode);
  void StopAll(FadeOut mode);
  bool GetLoadState() const;


private:

  // name of GUID file for FMOD
  const std::string _GUID;
  // pointer to the FMOD system
  SOUND::System* _system;
  // container for all sound events
  std::unordered_map<std::string, SoundEvent> _events;
  // vector for all sound banks
  std::vector<SOUND::Bank *> _banks;

  void** _ninjaPointer; // just in case

};
