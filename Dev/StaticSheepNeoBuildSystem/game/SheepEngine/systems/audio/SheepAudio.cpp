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

#include <boost/filesystem.hpp>

#include <fstream>
#include <iostream>

// lets call this a sound map
typedef std::unordered_map<std::string, Sound*> SoundMap;

// lets call this a bank vector
typedef std::vector<SOUND::Bank*> BankVector;

// static prototypes
static void ParseBanks(SOUND::System *system, std::ifstream &file, BankVector &bank);
static void ParseEvents(SOUND::System *system, std::ifstream &file, SoundMap &eventMap);
static void ParseFiles(FMOD::System* lowLevelSystem, SoundMap &eventMap);
static void LoadBank(SOUND::System *system, std::string &name, BankVector &bank);
static void LoadEvent(SOUND::System *system, std::string &name, SoundMap &events);

static FMOD::ChannelGroup* masterGroup;
static FMOD::DSP* dsp;

using namespace boost::filesystem;

namespace Framework
{
	// Global pointer to the audio system
	SheepAudio* AUDIO = NULL;

	SheepAudio::SheepAudio() : GUID("GUIDs.txt")
	{
    // set the global pointer 
		AUDIO = this;

    masterVolume = 1.0f;
    masterPitch = 1.0f;
	}

	SheepAudio::~SheepAudio()
	{
		TRACELOG->Log(TraceLevel::INFO,"Releasing FMOD Studio");
    system->release();
	}

  void SheepAudio::Shutdown()
  {
    TRACELOG->Log(TraceLevel::INFO,"Releasing all Sounds");
    for(auto it = soundMap.begin(); it != soundMap.end(); ++it)
    {
      delete it->second;
      it->second = 0;
    }

    delete debug;
  }

	void SheepAudio::Initialize()
	{
    TRACELOG->Log(TraceLevel::INFO,"Creating FMOD Studio System");
    ErrorCheck(SOUND::System::create(&system));

    TRACELOG->Log(TraceLevel::VERBOSE, "Initializing FMOD Studio System");
    ErrorCheck(system->initialize(256, FMOD_STUDIO_INIT_NORMAL, 
                      FMOD_INIT_NORMAL, 0));

    TRACELOG->Log(TraceLevel::VERBOSE, "Getting FMOD Low Level System");
    ErrorCheck(system->getLowLevelSystem(&lowLevelSystem));

    int driver;

    TRACELOG->Log(TraceLevel::INFO, "Checking Current Sound Drivers");
    ErrorCheck(lowLevelSystem->getNumDrivers(&driver));

    // if there is no soundcard/driver, tell FMOD to not even bother
    if(driver == 0)
    {
      TRACELOG->Log(TraceLevel::WARNING, "No Soundcard/Driver found");
      ErrorCheck(lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
    }

    // open the GUID file
    std::ifstream infile(SoundUtility::SourcePath(GUID, SoundUtility::TYPE_GUIDs).c_str());

    // if the file couldn't be opened... throw an exception
    if(!infile.is_open())
     throw std::invalid_argument("Invalid File"); // replace with event handling system

    // parse through the GUID file and load the banks and events
    TRACELOG->Log(TraceLevel::VERBOSE, "Parsing Bank Files");
    ParseBanks(system, infile, banks);

    TRACELOG->Log(TraceLevel::VERBOSE, "Parsing Sound Events");
    ParseEvents(system, infile, soundMap);

    TRACELOG->Log(TraceLevel::VERBOSE, "Parsing Sound Files");
    ParseFiles(lowLevelSystem, soundMap);;

    TRACELOG->Log(TraceLevel::VERBOSE, "Getting Master Channel Group");
    ErrorCheck(lowLevelSystem->getMasterChannelGroup(&masterGroup));

    // create a dsp, and add it to the masterGroup for debug information right now
    ErrorCheck(lowLevelSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp));
    ErrorCheck(masterGroup->addDSP(0, dsp));
    ErrorCheck(masterGroup->setVolume(1.0f));

    debug = new DebugAudio;
	}

  void SheepAudio::RegisterComponents(void)
  {
    REGISTER_COMPONENT(SoundEmitter);
    REGISTER_COMPONENT(SoundPlayer);
  }

	void SheepAudio::Update(float dt)
	{
    float temp = dt; // get rid of warning

    // update all of the sounds
    ErrorCheck(system->update());
    return;
	}

  // Looks for the sound, and plays it based on the SoundInstance parameters if it was found.
  bool SheepAudio::Play(const std::string &event_name, SoundInstance* instance)
  {
    auto it = soundMap.find(event_name);

    // if found, tell this event to play
    if(it != soundMap.end())
      return soundMap[event_name]->Play(instance);
    return false;
  }

  // Stops a specific instance of a sound
  bool SheepAudio::Stop(SoundInstance* instance)
  {
    // tell this event to stop
    if(instance->type == 0)
    {
     if(ErrorCheck(instance->eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE)))
      return false;
    }

    if(instance->type == 1)
    {
     if(ErrorCheck(instance->soundInstance->release()))
      return false;
    }

    return true;
  }

  // Pauses a specific instance of a sound
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

  // Pauses all sounds
  void SheepAudio::PauseAll(bool status)
  {
    masterGroup->setPaused(status);
  }

  // Stops all sounds from playing
  void SheepAudio::StopAll()
  {
    masterGroup->stop();
    return;
  }

  // Sets master volume. Gets clamped between 0.0 and 1.0
  void SheepAudio::SetMasterVolume(float volume)
  {
    masterVolume = Clamp(volume, 0.0f, 1.0f);
    masterGroup->setVolume(masterVolume);
  }
  
  // Get the master volume
  float SheepAudio::GetMasterVolume()
  {
    return masterVolume;
  }

  // Sets master pitch. Gets clamped between 0.0 and 1.0
  void SheepAudio::SetMasterPitch(float pitch)
  {
    masterPitch = Clamp(pitch, 0.0f, 2.0f);
    masterGroup->setPitch(masterPitch);
  }

  // Gets the master pitch
  float SheepAudio::GetMasterPitch()
  {
    return masterPitch;
  }

  // Checks FMOD to see if it done loading. Returns true if done, false if not.
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

  // Gets relevant data from FMOD and returns a void* to the data.
  const void* SheepAudio::GetDebugData()
  {
    ErrorCheck(system->getCPUUsage(&debug->cpuLoad));
    ErrorCheck(lowLevelSystem->getChannelsPlaying(&debug->channels));
    ErrorCheck(FMOD::Memory_GetStats(NULL, &debug->RAM, false));
    ErrorCheck(system->getBufferUsage(&debug->bufferInfo));

    //ErrorCheck(dsp->getParameterData(2, &debug->data, &debug->block, NULL, 0));

    return (void*)debug;
  }

} // end namespace


// ============================ STATIC FUNCTIONS =========================== //

// Parses through the GUID and looks for bank files to load
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
      Framework::TRACELOG->Log(Framework::TraceLevel::VERBOSE, "Loading %s", str.substr(startPos, endPos).append(".bank"));
      LoadBank(system, str.substr(startPos, endPos).append(".bank"), bank);
    }
  }

  // must load the unlisted master string bank
  LoadBank(system, str = "Master Bank.strings.bank", bank);
}

// Parses through the GUID for events to load.
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

      Framework::TRACELOG->Log(Framework::TraceLevel::VERBOSE, "Loading %s", str.substr(position, endPos));
      LoadEvent(system, str.substr(position, endPos), soundMap);
    }
  }
}

// Parses through the content folder for .wav, .mp3, and .aif files to load.
void ParseFiles(FMOD::System* system, SoundMap& soundMap)
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
          Framework::TRACELOG->Log(Framework::TraceLevel::VERBOSE, "Loading %s", name.substr(start + 1,std::string::npos));
          soundMap[name.substr(start + 1,end - 1)] = new SoundFile(system, it->path().generic_string(), flag);
        }
      }
    }
  }
  return;
}

// Loads the bank into FMOD and stores the pointer in a vector
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

// Creates an event and stores it in the sound map
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
