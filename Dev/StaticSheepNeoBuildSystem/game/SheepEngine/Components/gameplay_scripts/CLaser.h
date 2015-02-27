/*****************************************************************
Filename: CLaser.h
Project:  Giga Gravity Games
Author(s): Jon Sourbeer (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../transform/CTransform.h"
#include "components/sprites/CSprite.h"
#include "../colliders/CCircleCollider.h"


namespace Framework
{
  

  class Laser : public GameComponent
  {
  public:
    Laser();
    ~Laser();
    void LogicUpdate(float dt);
    void Initialize();
    void Remove();
    void SimpleCaster(CircleCollider *lc);
    void ComplexCaster(CircleCollider *lc);
    void ModifyPositionOffsets(void);

    void SetBodyTexture(const char * Texture);
    DirectSheep::Handle& GetBodyTexture();

    void SetBeamTexture(const char * Texture);
    DirectSheep::Handle& GetBeamTexture();

    void TweakSetLayer(const void* layerNum);
    void TweakSetBodyTexture(const void * Texture);
    void TweakSetBeamTexture(const void * Texture);

    void DrawLaser();

    void(Laser::*Caster)(CircleCollider *lc);

    //member variables
    Handle lTransfrom;
    Handle lCollider;
    Handle lEmitter;
    Handle lBeam;

    unsigned int type;

    float startDelay;   //the amount of time to wait before firing
    float duration;     //amount of time laser lasts once firing

    float damage;
    int width;  //the width of the total laser, will use an algorithm to determine # of rays to cast

    float arcRotation;  //the arc to trace out over the duration
    float arcDelay;     //the delay on the arc before firing
    float arcPerSec;

    std::vector<Vec3D> positionOffsets; //used for multiple raycasts
    int numberOfRays;

    // Texture names
    std::string m_bodyTexName;
    std::string m_beamTexName;

    // Texture handles
    DirectSheep::Handle m_bodyTex;
    DirectSheep::Handle m_beamTex;

    Vec2                m_bodyTexDim;
    Vec2                m_beamTexDim;

    Vec2                m_bodyScale;

    Vec4                m_bodyColor;
    Vec4                m_beamColor;

  };
}