#include "precompiled.h"
#include "SpineAtlas.h"
#include <fstream>
#include <string>


namespace DirectSheep
{

  SpineAtlas::SpineAtlas()
  {

  }

  SpineAtlas::~SpineAtlas()
  {

  }

  std::string& SpineAtlas::GetTextureName()
  {
    return texName;
  }
    
  void SpineAtlas::Load(const std::string& path)
  {
    std::fstream file(std::string(path), std::ifstream::in);

    if(file.is_open())
    {
      std::string buffer;
      std::string reader;

      std::getline(file, reader);
      texName = reader;
      //buffer = reader.substr(0, reader.find("."));

      std::getline(file, reader);

      while(!file.eof())
      {
        std::getline(file, reader);
        size_t begin, end;

        if(reader.find("entity:") != std::string::npos)
        {
          buffer = reader.substr(reader.find(":") + 2, std::string::npos);
        }

        if(reader.find("sequence:") != std::string::npos)
        {
          begin = reader.find(":") + 2;
          reader = reader.substr(begin, std::string::npos);
          int braces = 0;
          std::string temp;

          while(!file.eof())
          {
            std::getline(file, temp);

            if(temp.find("{") != std::string::npos)
            {
              ++braces;
              break;
            }
          }

          int frame = -1;

          while(braces)
          {
            std::getline(file, temp);

            if(temp.find("{") != std::string::npos)
            {
              ++braces;
              ++frame;
            }
            else
            if(temp.find("}") != std::string::npos)
              --braces;
            else
            if((begin = temp.find(":")) != std::string::npos)
            {
              std::string temp2 = temp.substr(begin + 2, temp.find(",") - (begin + 2));

              float u1 = (float)atof(temp2.c_str());
              float v1 = (float)atof(temp.substr(temp.find(",") + 2, std::string::npos).c_str());

              std::getline(file, temp);

              begin = temp.find(":");

              float u2 = (float)atof(temp.substr(begin + 2, temp.find(",") - (begin + 2)).c_str());
              float v2 = (float)atof(temp.substr(temp.find(",") + 2, std::string::npos).c_str());

              atlas[buffer][reader].sequence.push_back(FrameData(Framework::Vec2D(u1, v1), Framework::Vec2D(u2, v2)));
            }
          }
        }
      }
    }
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
  const SequenceData* SpineAtlas::GetSequence(std::string& entity, std::string& sequence)
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
        if( frame < seq->second.sequence.size())
          return &seq->second.sequence[frame];

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
