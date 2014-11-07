/******************************************************************************
Filename: SheepAudio.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "pch/precompiled.h"

#include "SheepAudio.h"
#include "../SheepUtil/include/SheepMath.h"

#include "components/sound/CSoundEmitter.h"
#include "components/sound/CSoundPlayer.h"
#include <boost/filesystem.hpp>

#include <fstream>
#include <iostream>

// lets just call this an event map...
typedef std::unordered_map<std::string, Sound*> SoundMap;

// and this a vector of banks... bank pointers really
typedef std::vector<SOUND::Bank *> BankVector;

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
	// Global pointer
	SheepAudio* AUDIO = NULL;

/*****************************************************************************/
/*!
  \brief
    Default constructor for the SheepAudio class
*/
/*****************************************************************************/
	SheepAudio::SheepAudio() : GUID("GUIDs.txt") // need to find the GUIDs file
	{
    // set the global pointer 
		AUDIO = this;

    masterVolume = 1.0f;

    // need to read in config files for volume settings later...
    // but for now just set everything to max volume
	}

/*****************************************************************************/
/*!
  \brief
    Destructor for the SheepAudio class, all it does is release the FMOD
    system.
*/
/*****************************************************************************/
	SheepAudio::~SheepAudio()
	{
		// Release the FMOD system
    system->release();

	}

  void SheepAudio::Shutdown()
  {
    for(auto it = soundMap.begin(); it != soundMap.end(); ++it)
    {
      delete it->second;
      it->second = 0;
    }
  }

/*****************************************************************************/
/*!
  \brief
    Initializes the audio system. Loads all of the banks and events.
*/
/*****************************************************************************/
	void SheepAudio::Initialize()
	{
      // create the sound system
    ErrorCheck(SOUND::System::create(&system));

    // initialize the sound system, with 512 channels... NEVER RUN OUT
    ErrorCheck(system->initialize(256, FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE, 
                                        FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE, 0));
    ErrorCheck(system->getLowLevelSystem(&lowLevelSystem));

    // open the GUID file
    std::ifstream infile(SoundUtility::SourcePath(GUID, SoundUtility::TYPE_GUIDs).c_str());

    // if the file couldn't be opened... throw an exception
    if(!infile.is_open())
      throw std::invalid_argument("Invalid File"); // replace with event handling system

    // parse through the GUID file and load the banks and events
    ParseBanks(system, infile, banks);
    ParseEvents(system, infile, soundMap);
    ParseFiles(lowLevelSystem, soundMap);;

    ErrorCheck(lowLevelSystem->getMasterChannelGroup(&masterGroup));
    ErrorCheck(lowLevelSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &dsp));

    ErrorCheck(masterGroup->addDSP(0, dsp));

    ErrorCheck(masterGroup->setVolume(1.0f));

    /*
    SoundInstance instance;
    instance.mode = PLAY_LOOP;
    soundMap["topgun"]->Play(&instance);*/

    debug = new DebugAudio;
	}

  void SheepAudio::RegisterComponents(void)
  {
    REGISTER_COMPONENT(SoundEmitter);
    REGISTER_COMPONENT(SoundPlayer);
  }

/*****************************************************************************/
/*!
  \brief
    Updates the audio system. All it does it update FMOD.
*/
/*****************************************************************************/
	void SheepAudio::Update(float dt)
	{
    float temp = dt; // get rid of warning

    // update all of the sounds
    ErrorCheck(system->update());
    return;
	}

/*****************************************************************************/
/*!
  \brief
    Plays a sound!

  \param event_name
    The event name that we want to play. This is a string.

  \param mode
    How we want to play the sound. Single-shot, looped, or streamed.
*/
/*****************************************************************************/
  bool SheepAudio::Play(const std::string &event_name, SoundInstance* instance)
  {
    // tell this event to play
    return soundMap[event_name]->Play(instance);
  }

/*****************************************************************************/
/*!
  \brief
    Tells and event to stop playing.

  \param event_name
    The event that we want to stop playing.

  \param mode
    How we want to fade out when we stop... currently only use 0 or 1.
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Tells all events to stop playing.

  \param mode
    How we want to fade out. Use either 0 or 1 for now...
*/
/*****************************************************************************/
  void SheepAudio::StopAll()
  {
    // iterate through all events and stop all of them
    masterGroup->stop();

    return;
  }

  void SheepAudio::SetMasterVolume(float volume)
  {
    masterVolume = Clamp(volume, 0.0f, 1.0f);
    masterGroup->setVolume(masterVolume);
  }

  float SheepAudio::GetMasterVolume()
  {
    return masterVolume;
  }

  void SheepAudio::SetMasterPitch(float pitch)
  {
    masterPitch = Clamp(pitch, 0.0f, 1.0f);
    masterGroup->setPitch(masterPitch);
  }

  float SheepAudio::GetMasterPitch()
  {
    return masterPitch;
  }

/*****************************************************************************/
/*!
  \brief
    This function checks to see if the FMOD system is done loading banks.
*/
/*****************************************************************************/
  bool SheepAudio::GetLoadState() const
  {
      for (auto it = banks.begin(); it != banks.end(); ++it)
      {
          FMOD_STUDIO_LOADING_STATE state;

          ErrorCheck( (*it)->getSampleLoadingState(&state) );

          if (state != FMOD_STUDIO_LOADING_STATE_LOADED)
              return false;
      }

      return true;
  }

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






/*****************************************************************************/
//        END NAMESPACE... ENTER STATIC FUNCTIONS
/*****************************************************************************/


/*****************************************************************************/
/*!
  \brief
    Opens up the guid.txt file, parses through the file to find the location
    of the .bank and .event files to load them.

  \param system
    Pointer to the FMOD system

  \param file
    Reference to the guid.txt file

  \param bank
    Reference to the vector of bank files. (All of the sound files)
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Parses through the guid.txt file to find the event files to load later.

  \param system
    Pointer to the FMOD system

  \param file
    Reference to the guid.txt file

  \param event
    Reference to an unordered map of events that we will be loading the events
    into.
*/
/*****************************************************************************/
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

void ParseFiles(FMOD::System* system, SoundMap& soundMap)
{
  path p("content\\Audio\\");

  if (exists(p))
  {
    if (is_directory(p))
    {
      for (directory_iterator it(p), end; it != end; ++it)
      {
        if (it->path().extension().generic_string() == ".wav" || it->path().extension().generic_string() == ".mp3")
        {
          int size = file_size(it->path().generic_string());
          bool flag = false;

          // if it is larger than a megabyte, load it as a stream instead
          if(size > 1000000)
          {
            flag = true;
          }

          std::string name = it->path().generic_string();
          std::size_t start = name.find_last_of("/");
          std::size_t end = name.find_last_of(".") - start;

          // cut out the directories and the extension for putting into the map
          soundMap[name.substr(start + 1,end - 1)] = new SoundFile(system, it->path().generic_string(), flag);
        }
      }
    }
  }
  return;
}


/*****************************************************************************/
/*!
  \brief
    Loads the vector of bank files.

  \param system
    Pointer to the FMOD system

  \param name
    Name of the .bank file to load

  \param bank
    Reference to the vector of bank files. (Really just pointers...)
*/
/*****************************************************************************/
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

/*****************************************************************************/
/*!
  \brief
    Loads an event into the EventMap

  \param system
    Pointer to the FMOD system.

  \param name
    Name of the event... this will also be the key to access the event.

  \param events
    Reference to the unordered map of events to load into.
*/
/*****************************************************************************/
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

  // we can now access the event by "Folder/Event
  // example... Music/TopGun... or with the EventString defines... MUSIC_TOPGUN

  // then gtfo

  
  return;
}



