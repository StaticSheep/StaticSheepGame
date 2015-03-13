#include "pch/precompiled.h"
#include "CSpineSprite.h"
#include "systems/graphics/SheepGraphics.h"
#include "systems/graphics/Debug.h"
#include "systems/input/input.h"
#include "../SheepUtil/include/Matrix2D.h"

namespace Framework
{
  SpineSprite::SpineSprite() : color(1.0f, 1.0f, 1.0f, 1.0f)
  {
    sequenceName = std::string("idle"); // note to self, every animation needs idle
    atlasName = std::string("master");
    entityName = std::string("ruiser");
    framerate = 20.0f;
  }

  SpineSprite::~SpineSprite()
  {
  }

  void SpineSprite::Initialize()
  {
    previousEntity = "none";
    previousSequence = "none";

    transform = this->GetOwner()->GetComponentHandle(eTransform);
    atlasHandle = GRAPHICS->GetAtlasHandle(atlasName);
    textureHandle = GRAPHICS->LoadTexture(GRAPHICS->RC()->GetAtlasTexture(atlasHandle));

    sheet = (GRAPHICS->RC())->GetAnimationSheet(atlasHandle, entityName);

    space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(SpineSprite::FrameUpdate));
    space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(SpineSprite::LogicUpdate));
    space->hooks.Add("Draw", self, BUILD_FUNCTION(SpineSprite::Draw));

    if(sheet)
    {
      sequenceIt = &sheet->find(sequenceName)->second;
      sequenceFrames = sequenceIt->sequence.size();
      offset.x = sequenceIt->offset.x;
      offset.y = sequenceIt->offset.y;
    }

    currentTime = 0;
    currentFrame = 0;
    flipX = false;
    flipY = false;
  }

  void SpineSprite::FrameUpdate(float dt)
  {
    if(ENGINE->m_editorActive && !ENGINE->PlayingInEditor())
    {
      if(previousEntity != entityName)
      {
        atlasHandle = GRAPHICS->GetAtlasHandle(atlasName);
        sheet = (GRAPHICS->RC())->GetAnimationSheet(atlasHandle, entityName);
      }

      LogicUpdate(dt);
    }
  }

  void SpineSprite::LogicUpdate(float dt)
  {
    currentTime += dt;

    if(currentTime > 1.0f / framerate)
    {
      if(!complexLoop)
      {
        ++currentFrame;
        currentFrame = currentFrame % sequenceFrames;
        currentTime = 0.0f;
      }
      else
      {
        ++currentFrame;
        currentFrame = (currentFrame % (endLoop - startLoop)) + startLoop;
        currentTime = 0.0f;
      }
    }
  }

  void SpineSprite::Draw()
  {
    if(!sheet)
    {
      atlasHandle = GRAPHICS->GetAtlasHandle(atlasName);
      sheet = (GRAPHICS->RC())->GetAnimationSheet(atlasHandle, entityName);
      return;
    }

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);

    Vec2 begin;
    Vec2 end;
    Vec2 frameOffset;
    Vec3 position = trans->GetTranslation();
    Vec3 scale = trans->GetScale();
    float theta = trans->GetRotation();

    Mat2D rot(theta);

    if(previousSequence != sequenceName)
    {
      sequenceIt = &sheet->find(sequenceName)->second;
      sequenceFrames = sequenceIt->sequence.size();

      previousSequence = sequenceName;
    }

    if(sequenceIt)
    {
      begin = (*sequenceIt).sequence[currentFrame].start_uv;
      end = (*sequenceIt).sequence[currentFrame].end_uv;
      frameOffset = (offset - (*sequenceIt).offset) * 0.5f;

      if(flipX)
        frameOffset = Vec2(-frameOffset.x, frameOffset.y);

      frameOffset = rot * frameOffset;

      ENGINE->TraceLog.Log(Framework::TraceLevel::DBG, "x %f, y %f", frameOffset.x, frameOffset.y);

      

      //frameOffset = (*sequenceIt).offset - offset;
    }
    else
    {
      begin = Vec2(0.0f, 0.0f);
      end = Vec2(1.0f, 1.0f);
    }
    
    GRAPHICS->SetPosition(position.x - frameOffset.x, position.y - frameOffset.y, position.z);
    GRAPHICS->SetRotation(theta);
    GRAPHICS->SetSize(scale.x, scale.y);
    GRAPHICS->SetColor(color);
    GRAPHICS->SetUV(begin, end);
    GRAPHICS->SetCamState(0);
    GRAPHICS->SetSpriteFlip(flipX, flipY);
    GRAPHICS->DrawBatched(textureHandle);
  }

  void SpineSprite::SetSequence(std::string& sequence, float fps)
  {
    framerate = fps;
    sequenceName = sequence;

    complexLoop = false;
  }

  void SpineSprite::SetComplexSequence(std::string& sequence, float fps, int loopStart, int loopEnd)
  {
    framerate = fps;
    sequenceName = sequence;

    startLoop = loopStart;
    endLoop = loopEnd;

    complexLoop = true;
  }

  void SpineSprite::SetColor(Vec4& color_)
  {
    color = color_;
  }

  void SpineSprite::FlipX(bool flip)
  {
    flipX = flip;
  }

  void SpineSprite::FlipY(bool flip)
  {
    flipY = flip;
  }

  void SpineSprite::Remove()
  {
    space->hooks.Remove("Draw", self);
    space->hooks.Remove("FrameUpdate", self);
    space->hooks.Remove("LogicUpdate", self);

    sheet = nullptr;
  }
}