#include "pch/precompiled.h"
#include "CSpineSprite.h"
#include "systems/graphics/SheepGraphics.h"
#include "systems/graphics/Debug.h"
#include "systems/input/input.h"


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
      sequenceIt = &sheet->find(sequenceName)->second.sequence;
      sequenceFrames = sequenceIt->size();
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

    if(previousSequence != sequenceName)
    {
      sequenceIt = &sheet->find(sequenceName)->second.sequence;
      sequenceFrames = sequenceIt->size();

      previousSequence = sequenceName;
    }

    if(sequenceIt)
    {
      begin = (*sequenceIt)[currentFrame].start_uv;
      end = (*sequenceIt)[currentFrame].end_uv;
    }
    else
    {
      begin = Vec2(0.0f, 0.0f);
      end = Vec2(1.0f, 1.0f);
    }
    
    GRAPHICS->SetPosition(trans->GetTranslation().x, trans->GetTranslation().y, trans->GetTranslation().z);
    GRAPHICS->SetRotation(trans->GetRotation());
    GRAPHICS->SetSize(trans->GetScale().x, trans->GetScale().y);
    GRAPHICS->SetColor(Vec4(color.r, color.g, color.b, color.a));
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