/******************************************************************************
Filename: CSpineLight.cpp
Project:  GAM 250
Author(s): Zachary Nawar (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "pch/precompiled.h"
#include "CSpineLight.h"
#include "systems/graphics/SheepGraphics.h"
#include "systems/graphics/Debug.h"
#include "systems/input/input.h"
#include "../SheepUtil/include/Matrix2D.h"
#include "../sprites/CSpineSprite.h"

namespace Framework
{
  SpineLight::SpineLight() : Color(1.0f, 1.0f, 1.0f, 1.0f)
  {
    sequenceName = std::string("idle"); // note to self, every animation needs idle
    atlasName = std::string("ninja");
    entityName = std::string("ninja");
    framerate = 20.0f;
  }

  SpineLight::~SpineLight()
  {
  }

  void SpineLight::Initialize()
  {
    previousEntity = "none";
    previousSequence = "none";

    transform = this->GetOwner()->GetComponentHandle(eTransform);
    spineSprite = this->GetOwner()->GetComponentHandle(eSpineSprite);

    atlasHandle = GRAPHICS->GetAtlasHandle(atlasName);
    textureHandle = GRAPHICS->LoadTexture(GRAPHICS->RC()->GetAtlasTexture(atlasHandle));

    sheet = (GRAPHICS->RC())->GetAnimationSheet(atlasHandle, entityName);

    //space->hooks.Add("FrameUpdate", self, BUILD_FUNCTION(SpineLight::FrameUpdate));
    //space->hooks.Add("LogicUpdate", self, BUILD_FUNCTION(SpineLight::LogicUpdate));
    
    if (m_layer == 0)
    {
      space->hooks.Add("PreDraw", self, BUILD_FUNCTION(SpineLight::Draw));
    }
    else if (m_layer == 1)
    {
      space->hooks.Add("Draw", self, BUILD_FUNCTION(SpineLight::Draw));
    }
    else
    {
      space->hooks.Add("PostDraw", self, BUILD_FUNCTION(SpineLight::Draw));
    }
    

    //if(sheet)
    //{
    //  sequenceIt = &sheet->find(sequenceName)->second;
    //  sequenceFrames = sequenceIt->sequence.size();
    //  offset.x = sequenceIt->offset.x;
    //  offset.y = sequenceIt->offset.y;
    //}

    currentTime = 0;
    currentFrame = 0;
    flipX = false;
    flipY = false;
    doneLoop = false;
    finishedComplex = true;
  }

  void SpineLight::FrameUpdate(float dt)
  {
    if(ENGINE->m_editorActive && !ENGINE->PlayingInEditor())
    {
      if(previousEntity != entityName)
      {
        atlasHandle = GRAPHICS->GetAtlasHandle(atlasName);
        sheet = (GRAPHICS->RC())->GetAnimationSheet(atlasHandle, entityName);
        previousEntity = entityName;
      }

      LogicUpdate(dt);
    }
  }

  void SpineLight::LogicUpdate(float dt)
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
        int temp = endLoop - startLoop;

        if(temp == 0)
          currentFrame = startLoop;
        else
        if(currentFrame < startLoop)
          ++currentFrame;
        else
        {
          ++currentFrame;
          currentFrame = currentFrame % temp;
          doneLoop = true;
        }
        currentTime = 0.0f;
      }
    }
  }

  void SpineLight::Draw()
  {
    if(!sheet)
    {
      atlasHandle = GRAPHICS->GetAtlasHandle(atlasName);
      sheet = (GRAPHICS->RC())->GetAnimationSheet(atlasHandle, entityName);
      return;
    }

    Transform* trans = space->GetHandles().GetAs<Transform>(transform);
    SpineSprite* sp = space->GetHandles().GetAs<SpineSprite>(spineSprite);

    Vec2 begin;
    Vec2 end;
    Vec2 frameOffset;
    Vec3 position = trans->GetTranslation();
    Vec3 scale = trans->GetScale();
    scale.x *= m_lightingScale.x;
    scale.y *= m_lightingScale.y;
    float theta = trans->GetRotation();

    Mat2D rot(theta);

    if (previousSequence != sp->sequenceName)
    {
      sequenceIt = &sheet->find(sp->sequenceName)->second;
      sequenceFrames = sequenceIt->sequence.size();

      previousSequence = sp->sequenceName;
    }

    if(sequenceIt)
    {
      if (!sp->complexLoop)
      {
        begin = (*sequenceIt).sequence[sp->currentFrame].start_uv;
        end = (*sequenceIt).sequence[sp->currentFrame].end_uv;
      }
      else
      {

        //int temp = endLoop - startLoop;

        //begin = (*sequenceIt).sequence[(currentFrame % temp) + startLoop].start_uv;
        ///end = (*sequenceIt).sequence[(currentFrame % temp) + startLoop].end_uv;

        if (sp->doneLoop)
        {
          begin = (*sequenceIt).sequence[sp->currentFrame + sp->startLoop].start_uv;
          end = (*sequenceIt).sequence[sp->currentFrame + sp->startLoop].end_uv;
        }
        else
        {
          begin = (*sequenceIt).sequence[sp->currentFrame].start_uv;
          end = (*sequenceIt).sequence[sp->currentFrame].end_uv;
        }
      }

      frameOffset = (sp->offset - (*sequenceIt).offset) * 0.5f;

      if(sp->flipX)
        frameOffset = Vec2(-frameOffset.x, frameOffset.y);

      frameOffset = rot * frameOffset;
    }
    else
    {
      begin = Vec2(0.0f, 0.0f);
      end = Vec2(1.0f, 1.0f);
    }
    
    GRAPHICS->SetPosition(position.x - frameOffset.x, position.y - frameOffset.y, position.z);
    GRAPHICS->SetRotation(theta);
    GRAPHICS->SetSize(scale.x, scale.y);
    GRAPHICS->SetColor(Color);
    GRAPHICS->SetUV(begin, end);
    GRAPHICS->SetCamState(0);
    GRAPHICS->SetSpriteFlip(sp->flipX, sp->flipY);
    GRAPHICS->RC()->DrawLightBatched(textureHandle, m_emissive);
  }

  void SpineLight::SetSequence(std::string& sequence, float fps)
  {
    framerate = fps;
    sequenceName = sequence;
    doneLoop = false;
    complexLoop = false;
    finishedComplex = true;
  }

  void SpineLight::SetComplexSequence(std::string& sequence, float fps, int loopStart, int loopEnd)
  {
    framerate = fps;
    sequenceName = sequence;

    startLoop = loopStart;
    endLoop = loopEnd;

    complexLoop = true;

    if(finishedComplex)
    {
      finishedComplex = false;
      currentFrame = 0;
    }
  }

  void SpineLight::SetColor(Vec4& color_)
  {
    Color = color_;
  }

  void SpineLight::FlipX(bool flip)
  {
    flipX = flip;
  }

  void SpineLight::FlipY(bool flip)
  {
    flipY = flip;
  }

  void SpineLight::Remove()
  {
    space->hooks.Remove("Draw", self);
    space->hooks.Remove("FrameUpdate", self);
    space->hooks.Remove("LogicUpdate", self);

    space->hooks.Remove("PreDraw", self);
    space->hooks.Remove("PostDraw", self);

    sheet = nullptr;
  }
}