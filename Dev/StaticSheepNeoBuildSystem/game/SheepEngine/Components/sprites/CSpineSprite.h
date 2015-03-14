
#pragma once

#include "types/vectors/Vec2.h"
#include "components/base/Component.h"
#include "components/transform/CTransform.h"
#include "Handle.h"
#include "../SheepGraphics/Atlas/SpineAtlas.h"


namespace Framework
{
  class SpineSprite : public GameComponent
  {
    public:

      SpineSprite();
      ~SpineSprite();

      virtual void Initialize();

      virtual void Remove();

      void FrameUpdate(float dt);
      void LogicUpdate(float dt);
      void Draw();

      void SetSequence(std::string& sequence, float fps = 24.0f);
      void SetComplexSequence(std::string& sequence, float fps, int loopStart, int loopEnd);

      void SetColor(Vec4& color);
      void FlipX(bool flip);
      void FlipY(bool flip);

      std::string atlasName;
      std::string entityName;
      std::string sequenceName;

      float framerate;
      Vec4 Color;

    private:

      Handle transform;
      const DirectSheep::AnimationSheet* sheet;
      float currentTime;
      int currentFrame;

      int sequenceFrames;

      bool flipX;
      bool flipY;
      bool complexLoop;
      int startLoop;
      int endLoop;

      std::string previousSequence;
      std::string previousEntity;

      DirectSheep::Handle atlasHandle;
      DirectSheep::Handle textureHandle;
      //std::unordered_map<std::string, DirectSheep::SequenceData>::const_iterator sequenceIt;
      const DirectSheep::SequenceData* sequenceIt;

      
      Vec2 offset;
      
  };
}