/*****************************************************************
Filename: SheepAudio.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"

#include "SheepAudio.h"
#include "../SheepUtil/include/SheepMath.h"

#include "components/sound/CSoundEmitter.h"
#include "components/sound/CSoundPlayer.h"
#include "../input/Input.h"

#include <boost/filesystem.hpp>

#include <fstream>
#include <iostream>

// lets just call this a SoundMap
typedef std::unordered_map<std::string, Sound*> SoundMap;

// and this a vector of banks... bank pointers really
typedef std::vector<SOUND::Bank*> BankVector;

// static prototypes
static void ParseBanks(SOUND::System *system, std::ifstream &file, BankVector &bank);
static void ParseEvents(SOUND::System *system, std::ifstream &file, SoundMap &eventMap);
static void ParseFiles(FMOD::System* lowLevelSystem, SoundMap &eventMap, FMOD::ChannelGroup* musicChannel, FMOD::ChannelGroup* sfxChannel);
static void LoadBank(SOUND::System *system, std::string &name, BankVector &bank);
static void LoadEvent(SOUND::System *system, std::string &name, SoundMap &events);

static FMOD::ChannelGroup* masterGroup;
static FMOD::DSP* dsp;

using namespace boost::filesystem;

namespace Framework
{
	// Global pointer
	SheepAudio* AUDIO = NULL;


   
  // Default constructor for the SheepAudio class
	SheepAudio::SheepAudio() : GUID("GUIDs.txt") // need to find the GUIDs file
	{
    // set the global pointer 
		AUDIO = this;

    masterVolume = 1.0f;
    musicVolume = 1.0f;
    sfxVolume = 1.0f;

    masterPitch = 1.0f;
	}

	SheepAudio::~SheepAudio()
	{
		// Release the FMOD system
    system->release();
	}

  // Cleans up all dynamically allocated memory for SheepAudio
  void SheepAudio::Shutdown()
  {
    // kill all sound objects
    for(auto it = soundMap.begin(); it != soundMap.end(); ++it)
    {
      delete it->second;
      it->second = 0;
    }

    // delete the debug struct
    delete debug;
  }

  // Initializes the FMOD systems, loads banks, loads sound files
	void SheepAudio::Initialize()
	{
    // create the sound system
    ErrorCheck(SOUND::System::create(&system));

    // initialize the sound system, with 512 channels... NEVER RUN OUT
    ErrorCheck(system->initialize(256, FMOD_STUDIO_INIT_NORMAL, 
                      FMOD_INIT_NORMAL, 0));
    ErrorCheck(system->getLowLevelSystem(&lowLevelSystem));

    int driver;

    ErrorCheck(lowLevelSystem->getNumDrivers(&driver));

    // if there is no soundcard/driver, tell FMOD to not even bother
    if(driver == 0)
     ErrorCheck(lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND));

    // open the GUID file
    std::ifstream infile(SoundUtility::SourcePath(GUID, SoundUtility::TYPE_GUIDs).c_str());

    // if the file couldn't be opened... throw an exception
    if(!infile.is_open())
     throw std::invalid_argument("Invalid File"); // replace with event handling system

    ErrorCheck(lowLevelSystem->getMasterChannelGroup(&masterGroup));

    // create a dsp, and add it to the masterGroup for debug information right now
    ErrorCheck(lowLevelSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp));
    ErrorCheck(masterGroup->addDSP(0, dsp));
    ErrorCheck(masterGroup->setVolume(1.0f));

    ErrorCheck(lowLevelSystem->createChannelGroup("Music", &musicChannel));
    ErrorCheck(lowLevelSystem->createChannelGroup("SFX", &sfxChannel));

    ErrorCheck(masterGroup->addGroup(musicChannel));
    ErrorCheck(masterGroup->addGroup(sfxChannel));

    // parse through the GUID file and load the banks and events
    ParseBanks(system, infile, banks);
    ParseEvents(system, infile, soundMap);

    // parse through the content directory and load sound files
    ParseFiles(lowLevelSystem, soundMap, musicChannel, sfxChannel);

    // grab the master channel group for pausing, stopping, and altering volume
    

    debug = new DebugAudio;
	}

  // Registers all sound components with the engine
  void SheepAudio::RegisterComponents(void)
  {
    REGISTER_COMPONENT(SoundEmitter);
    REGISTER_COMPONENT(SoundPlayer);
  }

  void SheepAudio::ReceiveMessage(Message& msg)
  {
    if(msg.MessageId == Message::WindowMinimize)
    {
      PauseAll(true);
    }
    if(msg.MessageId == Message::WindowRestore)
    {
      PauseAll(false);
    }
  }

  // Updates FMOD every frame
	void SheepAudio::Update(float dt)
	{
    float temp = dt; // get rid of warning

    // update all of the sounds
    ErrorCheck(system->update());


    if(SHEEPINPUT->KeyIsPressed('N'))
    {
      SetMusicVolume(GetMusicVolume() - 0.1f);
      TRACELOG->Log(TraceLevel::DBG, "Music Volume %f", GetMusicVolume());
    }

    if(SHEEPINPUT->KeyIsPressed('M'))
    {
      SetMusicVolume(GetMusicVolume() + 0.1f);
      TRACELOG->Log(TraceLevel::DBG, "Music Volume %f", GetMusicVolume());
    }

    if(SHEEPINPUT->KeyIsPressed('J'))
    {
      SetSFXVolume(0.0f);
      TRACELOG->Log(TraceLevel::DBG, "SFX Volume %f", GetSFXVolume());
    }

    if(SHEEPINPUT->KeyIsPressed('K'))
    {
      SetSFXVolume(1.0f);
      TRACELOG->Log(TraceLevel::DBG, "SFX Volume %f", GetSFXVolume());
    }


    return;
	}

  // Plays the requested sound given by event_name, instance holds parameters
  bool SheepAudio::Play(const std::string &event_name, SoundInstance* instance)
  {
    auto it = soundMap.find(event_name);

    
    // tell this event to play
    if(it != soundMap.end())
    {
      if(instance->mode == PLAY_ONCE)
      {
        instance->volume *= masterVolume * sfxVolume;
      }
      else
      {
        instance->volume *= masterVolume * musicVolume;
      }

      return soundMap[event_name]->Play(instance);
    }
    return false;
  }

  // Stops the instance from playing, frees the channel
  bool SheepAudio::Stop(SoundInstance* instance)
  {
    // tell this event to stop
    if(instance->type == 0)
    {
      if (ErrorCheck(instance->eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE)))
      {
        instance->eventInstance->release();
        return false;
      }
    }

    if(instance->type == 1)
    {
      FMOD::Channel* chnl = nullptr;
      lowLevelSystem->getChannel(instance->channel, &chnl);
      if (chnl)
        chnl->stop();

      if (instance->soundInstance)
        if (ErrorCheck(instance->soundInstance->release()))
          return false;
    }

    return true;
  }

  // Pauses the instance. Channel remains in use
  void SheepAudio::Pause(SoundInstance* instance, bool status)
  {
    if(instance->type == 0)
     instance->eventInstance->setPaused(status);

    if(instance->type == 1)
    {
     FMOD::Channel* channel;
     lowLevelSystem->getChannel(instance->channel, &channel);
     channel->setPaused(status);
    }
  }

  // Stops the master channel group. Stops all sounds immediately
  void SheepAudio::StopAll()
  {
    // Stop everything
    masterGroup->stop();
    return;
  }

  // Pauses everything
  void SheepAudio::PauseAll(bool paused)
  {
    masterGroup->setPaused(paused);
  }

  // Setters
  void SheepAudio::SetMasterVolume(float volume)
  {
    masterVolume = Clamp(volume, 0.0f, 1.0f);
    masterGroup->setVolume(masterVolume);
  }

  void SheepAudio::SetMasterPitch(float pitch)
  {
    masterPitch = Clamp(pitch, 0.0f, 2.0f);
    masterGroup->setPitch(masterPitch);
  }

  void SheepAudio::SetMusicVolume(float volume)
  {
    musicVolume = Clamp(volume, 0.0f, 1.0f);
    musicChannel->setVolume(musicVolume);
  }

  void SheepAudio::SetSFXVolume(float volume)
  {
    sfxVolume = Clamp(volume, 0.0f, 1.0f);
    sfxChannel->setVolume(sfxVolume);
  }
    
  // Getters
  float SheepAudio::GetMasterVolume()
  {
    return masterVolume;
  }

  float SheepAudio::GetMusicVolume()
  {
    return musicVolume;
  }

  float SheepAudio::GetSFXVolume()
  {
    return sfxVolume;
  }

  float SheepAudio::GetMasterPitch()
  {
    return masterPitch;
  }

  // Returns true if FMOD is done loading, false if not
  bool SheepAudio::GetLoadState() const
  {
    for(auto it = banks.begin(); it != banks.end(); ++it)
    {
     FMOD_STUDIO_LOADING_STATE state;

     ErrorCheck((*it)->getSampleLoadingState(&state));

     if(state != FMOD_STUDIO_LOADING_STATE_LOADED)
      return false;
    }

    return true;
  }

  // Grabs relevant debug data from FMOD
  const void* SheepAudio::GetDebugData()
  {
    ErrorCheck(system->getCPUUsage(&debug->cpuLoad));
    ErrorCheck(lowLevelSystem->getChannelsPlaying(&debug->channels));
    ErrorCheck(FMOD::Memory_GetStats(NULL, &debug->RAM, false));
    ErrorCheck(system->getBufferUsage(&debug->bufferInfo));

    //ErrorCheck(dsp->getParameterData(2, &debug->data, &debug->block, NULL, 0));

    return (void*)debug;
  }
  
  // Lua bind for pausing everything
  void SheepAudio::LuaPauseAll(bool paused)
  {
    AUDIO->PauseAll(paused);
  }

} // end namespace

/*****************************************************************************/
//        END NAMESPACE... ENTER STATIC FUNCTIONS
/*****************************************************************************/

// Parses through the GUIDs.txt, loads all banks
void ParseBanks(SOUND::System *system, std::ifstream &file, BankVector &bank)
{
  // string for extraction
  std::string str;

   // must start at the beginning of the file
  file.clear();
  file.seekg(0, file.beg);

  // using getline to retain whitespaces
  while (std::getline(file, str))
  {
    std::size_t position = str.find("bank:/");

    if (position != std::string::npos)
    {
      // eleminate the prefix "bank:/"
      std::size_t prefix = std::string("bank:/").length();
      // finding the substring start and end
      std::size_t startPos = position + prefix;
      std::size_t endPos = str.length() - startPos;

      // loading the bank (substring)
      LoadBank(system, str.substr(startPos, endPos).append(".bank"), bank);
    }
  }

  // must load the unlisted master string bank
  LoadBank(system, str = "Master Bank.strings.bank", bank);
}

// Parses the GUIDs.txt, loads all events
void ParseEvents(SOUND::System *system, std::ifstream &file, SoundMap &soundMap)
{
  // string for extraction
  std::string str;

  // must start at the beginning of the file
  file.clear();
  file.seekg(0, file.beg);

  // using getline to retain whitespaces
  while (std::getline(file, str))
  {
    std::size_t position = str.find("event:/");

    if (position != std::string::npos)
    {
      // finding the substrings end
      std::size_t endPos = str.length() - position;

      // loading the event (substring)
      LoadEvent(system, str.substr(position, endPos), soundMap);
    }
  }
}

// Searches through content directory and loads appropriate sound files
void ParseFiles(FMOD::System* system, SoundMap& soundMap, FMOD::ChannelGroup* musicChannel, FMOD::ChannelGroup* sfxChannel)
{
  path p("content\\Audio\\");

  // if the folder exists.
  if (exists(p))
  {
    // and is an actual directory
    if (is_directory(p))
    {
      // search through it...
      for (directory_iterator it(p), end; it != end; ++it)
      {
        // if the current file is a .wav or .mp3
        if (it->path().extension().generic_string() == ".wav" || it->path().extension().generic_string() == ".mp3"
         || it->path().extension().generic_string() == ".aif")
        {
          // get it's size first
          int size = (int)file_size(it->path().generic_string());
          bool flag = false;

          // if it is larger than half a megabyte, load it as a stream instead
          if(size > 500000)
          {
            flag = true;
          }

          // get the full name, and then we will get the substring that is just it's name
          // minus the path and the extension
          std::string name = it->path().generic_string();
          std::size_t start = name.find_last_of("/");
          std::size_t end = name.find_last_of(".") - start;

          // cut out the directories and the extension for putting into the map
          soundMap[name.substr(start + 1,end - 1)] = new SoundFile(system, it->path().generic_string(), musicChannel, sfxChannel, flag);
        }
      }
    }
  }
  return;
}

// Loads the bank, keeps a pointer to it in the bank vector
void LoadBank(SOUND::System *system, std::string &name, BankVector &bank)
{
  SOUND::Bank *newBank = NULL;

  std::string pathName = SoundUtility::SourcePath(name, SoundUtility::TYPE_AUDIO);

  // load the bank file into memory, do non-blocking for asynchronous loading
  ErrorCheck(system->loadBankFile(pathName.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &newBank));
  ErrorCheck (newBank->loadSampleData());

  // push the pointer to the bank onto the vector
  bank.push_back(newBank);
  return;
}

// Loads an event, stores it into an unordered_map
void LoadEvent(SOUND::System *system, std::string &name, SoundMap &sounds)
{
  // create a new event...
  SoundEvent* newEvent = new SoundEvent(system, name);

  // lets get rid of the event:/ part...
  std::size_t pos = name.find("/");
  std::size_t endPos = name.length() - pos;

  std::string newName = name.substr((pos + 1), endPos);

  // and shove it into the map with the string name..
  sounds[newName] = newEvent;

  return;
}
