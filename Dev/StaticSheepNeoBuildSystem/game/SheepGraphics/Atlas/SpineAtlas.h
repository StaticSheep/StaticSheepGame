#pragma once
#include <vector>
#include <unordered_map>
#include <string>

namespace DirectSheep
{
  struct FrameData
  {
    Framework::Vec2D start_uv;
    Framework::Vec2D end_uv;
    Framework::Vec2D root;
  };

  typedef std::vector< FrameData > Sequence;
  typedef std::unordered_map< std::string, Sequence > AnimationSheet;
  typedef std::unordered_map< std::string, AnimationSheet > Atlas;



  // contains all of the uv data for all spine animations
  class SpineAtlas
  {
    public:
    
      SpineAtlas();
      ~SpineAtlas();
    
      void Load(const std::string& path);

      const AnimationSheet* GetAnimationSheet(std::string& entity);
      const Sequence* GetSequence(std::string& entity, std::string& sequence);
      const FrameData* GetFrame(std::string& entity, std::string& sequence, int frame);
    
    private:
    
      int textureID;
      Atlas atlas;
  };
}
