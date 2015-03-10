#include "precompiled.h"
#include "SpineAtlas.h"
#include <fstream>


namespace DirectSheep
{

  SpineAtlas::SpineAtlas()
  {

  }

  SpineAtlas::~SpineAtlas()
  {

  }
    
  void SpineAtlas::Load(const std::string& path)
  {
    std::fstream atlasFile(path, std::ifstream::in);

    if(atlasFile.is_open())
    {
      int poop = 10;
      poop = 12;

      std::string herp;

      atlasFile >> herp;

      atlasFile >> herp;

      poop = 13;

    }

    int poop = 10;
    poop = 12;
  }

  // Grabs a particular sprite sheet for an entity
  const AnimationSheet* SpineAtlas::GetAnimationSheet(std::string& entity)
  {
    // make sure the entity exists in the map
    auto it = atlas.find(entity);

    if(it != atlas.end())
      return &it->second;

    Framework::TRACELOG->Log(Framework::TraceLevel::ERR, "No animation entity found with name %s", entity);

    return nullptr;
  }

  // Grabs the animation sequence for an entity
  const Sequence* SpineAtlas::GetSequence(std::string& entity, std::string& sequence)
  {
    auto it = atlas.find(entity);

    if(it != atlas.end())
    {
      auto seq = it->second.find(sequence);

      if(seq != it->second.end())
        return &seq->second;

      Framework::TRACELOG->Log(Framework::TraceLevel::ERR, "No animation sequence found with name %s", sequence);
      return nullptr;
    }

    Framework::TRACELOG->Log(Framework::TraceLevel::ERR, "No animation entity found with name %s", entity);
    return nullptr;
  }

  // Grabs an individual frame of an entity's sequence
  const FrameData* SpineAtlas::GetFrame(std::string& entity, std::string& sequence, int frame)
  {
    auto it = atlas.find(entity);

    if(it != atlas.end())
    {
      auto seq = it->second.find(sequence);

      if(seq != it->second.end())
      {
        if( frame < seq->second.size())
          return &seq->second[frame];

        Framework::TRACELOG->Log(Framework::TraceLevel::ERR, "No animation frame found at %i", frame);
        return nullptr;
      }

      Framework::TRACELOG->Log(Framework::TraceLevel::ERR, "No animation sequence found with name %s", sequence);
      return nullptr;
    }

    Framework::TRACELOG->Log(Framework::TraceLevel::ERR, "No animation entity found with name %s", entity);
    return nullptr;
  }
}
