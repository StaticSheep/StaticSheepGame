
#pragma once

#include "types/vectors/Vec2.h"
#include "components/base/Component.h"
#include "components/transform/CTransform.h"
#include "Handle.h"
#include "../SheepGraphics/Atlas/SpineAtlas.h"


namespace Framework
{
  class SpineLight : public GameComponent
  {
    public:

      SpineLight();
      ~SpineLight();

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
      LightColor Color;
      
      Vec2 m_lightingScale = Vec2(1, 1);
      bool m_emissive = false;
      int m_layer = 1;

      bool active = true;

      void Toggle(bool status) { active = status; }
      bool GetStatus() { return active; }

    private:

      Handle transform;
      Handle spineSprite;

      const DirectSheep::AnimationSheet* sheet;
      float currentTime;
      int currentFrame;

      int sequenceFrames;

      bool flipX;
      bool flipY;
      bool complexLoop;
      bool doneLoop;
      bool finishedComplex;
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