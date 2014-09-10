/*****************************************************************
Filename: SheepAudio.cpp
Project: 
Author(s): Zakary Wilson

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "fmodStudioL_vc.lib")

#include "SheepAudio.h"
#include <fstream>

// lets just call this an event map...
typedef std::unordered_map<std::string, SoundEvent> EventMap;

// and this a vector of banks... bank pointers really
typedef std::vector<SOUND::Bank *> BankVector;

// static prototypes
static void ParseBanks(SOUND::System *system, std::ifstream &file, BankVector &bank);
static void ParseEvents(SOUND::System *system, std::ifstream &file, EventMap &eventMap);
static void LoadBank(SOUND::System *system, std::string &name, BankVector &bank);
static void LoadEvent(SOUND::System *system, std::string &name, EventMap &events);


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
	SheepAudio::SheepAudio() : _GUID("GUIDs.txt") // need to find the GUIDs file
	{
    // set the global pointer 
		AUDIO = this;
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
    _system->release();
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
    ErrorCheck(SOUND::System::create(&_system));

    // initialize the sound system, with 512 channels... NEVER RUN OUT
    ErrorCheck(_system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_STUDIO_INIT_NORMAL, 0) );

    // open the GUID file
    std::ifstream infile(SoundUtility::SourcePath(_GUID, SoundUtility::TYPE_GUIDs).c_str());

    // if the file couldn't be opened... throw an exception
    if(!infile.is_open())
      throw std::invalid_argument("Invalid File"); // replace with event handling system

    // parse through the GUID file and load the banks and events
    ParseBanks(_system, infile, _banks);
    ParseEvents(_system, infile, _events);
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

    if(_events["Music/TopGun"].PlayState() == 0)
      _events["Music/TopGun"].Play(PLAY_LOOP);

    // update all of the sounds
    ErrorCheck(_system->update());
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
  void SheepAudio::Play(const std::string &event_name, PlayMode mode)
  {
    // tell this event to play
    _events[event_name].Play(mode);
    return;
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
  void SheepAudio::Stop(const std::string &event_name, FadeOut mode)
  {
    // tell this event to stop
    _events[event_name].Stop(mode);
    return;
  }

/*****************************************************************************/
/*!
  \brief
    Tells all events to stop playing.

  \param mode
    How we want to fade out. Use either 0 or 1 for now...
*/
/*****************************************************************************/
  void SheepAudio::StopAll(FadeOut mode)
  {
    // iterate through all events and stop all of them
    for( auto it = _events.begin(); it != _events.end(); ++it)
    {
      it->second.Stop(mode); // hopefully this works
    }

    return;
  }

/*****************************************************************************/
/*!
  \brief
    This function checks to see if the FMOD system is done loading banks.
*/
/*****************************************************************************/
  bool SheepAudio::GetLoadState() const
  {
      for (auto it = _banks.begin(); it != _banks.end(); ++it)
      {
          FMOD_STUDIO_LOADING_STATE state;

          ErrorCheck( (*it)->getSampleLoadingState(&state) );

          if (state != FMOD_STUDIO_LOADING_STATE_LOADED)
              return false;
      }

      return true;
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
void ParseEvents(SOUND::System *system, std::ifstream &file, EventMap &eventMap)
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
      LoadEvent(system, str.substr(position, endPos), eventMap);
    }
  }
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
void LoadEvent(SOUND::System *system, std::string &name, EventMap &events)
{
  // create a new event...
  SoundEvent newEvent(system, name);

  // lets get rid of the event:/ part...
  std::size_t pos = name.find("/");
  std::size_t endPos = name.length() - pos;

  std::string newName = name.substr((pos + 1), endPos);

  // and shove it into the map with the string name..
  events[newName] = newEvent;

  // we can now access the event by "Folder/Event
  // example... Music/TopGun... or with the EventString defines... MUSIC_TOPGUN

  // then gtfo
  return;
}

