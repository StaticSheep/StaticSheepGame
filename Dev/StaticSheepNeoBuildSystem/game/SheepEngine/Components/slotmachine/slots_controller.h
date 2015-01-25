/******************************************************************************
Filename: slots_controller.h
Project:
Author(s): Zachary Nawar (primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once


#include "components\base\Component.h"

namespace Framework
{
  class Transform;

  class SlotController : public GameComponent
  {
  public:

    SlotController();
    ~SlotController();

    virtual void Initialize();
    virtual void Remove();

    void Update(float dt);
    

    float slotHeight = 64;

    float fastToSlow = 1.5f;
    float slowToCrawl = 1.0f;
    float crawlToStop = 2.0f;

    float startSpeed = 4;
    

  private:
    float m_curTime = 0;


    enum Stage
    {
      Stopped,
      Fast,
      Slow,
      Crawl,
      Stop
    };

    float m_uvSnap;
    float m_curSpeed;
    float m_timeLeft;

    float m_timeData[4];

    float m_startCrawlSpeed;

    bool m_transition;
    Stage m_stage;
  };
}