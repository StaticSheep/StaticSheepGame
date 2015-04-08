/******************************************************************************
Filename: SpineAtlas.h
Project:  GAM 250
Author(s): Zakary Nawar (primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once
#include <vector>
#include <unordered_map>
#include <string>

namespace DirectSheep
{
  struct FrameData
  {
    FrameData() : start_uv(), end_uv() {};
    FrameData(Framework::Vec2D& start, Framework::Vec2D& end) : start_uv(start), end_uv(end) {};

    Framework::Vec2D start_uv;
    Framework::Vec2D end_uv;
  };

  typedef std::vector< FrameData > Sequence;

  struct SequenceData
  {
    Framework::Vec2D offset;
    Sequence sequence;
  };

  typedef std::unordered_map< std::string, SequenceData > AnimationSheet;
  typedef std::unordered_map< std::string, AnimationSheet > Atlas;



  // contains all of the uv data for all spine animations
  class SpineAtlas
  {
    public:
    
      SpineAtlas();
      ~SpineAtlas();
    
      void Load(const std::string& path);
      std::string& GetTextureName();

      const AnimationSheet* GetAnimationSheet(std::string& entity);
      const SequenceData* GetSequence(std::string& entity, std::string& sequence);
      const FrameData* GetFrame(std::string& entity, std::string& sequence, int frame);
    
    private:
    
      std::string texName;
      Atlas atlas;
  };
}
