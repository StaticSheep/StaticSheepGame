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

  class SlotMachine : public GameComponent
  {
  public:
    SlotMachine();
    ~SlotMachine();

    virtual void Initialize();
    virtual void Remove();

    void TweakSetNumSlots(const void* value);

    void Update(float dt);
    void Draw();

    void Start();
    void Stop();

    bool startSpinning = true;

    int slotOptions = 3;
    int numSlots = 3;

    float startSpeed = 7;

    Vec2 slotSize = Vec2(1, 1);
    float slotMargin = 10;

    float startTime = 3.0f;
    float bonusTime = 0.2f;
    
    int m_order = 0;
  private:

    void SetupSlots();

    struct Slot
    {
      float timeLeft = 0;
      float curSpeed = 0;
      
      float spinPos = 0;

      float height = 0;
      float realHeight = 0;
      
      int land = -1;

      int spinTextureID = -1;
      int stopTextureID = -1;

      bool stopping = false;
      bool spinning = false;
      bool timed = false;
    };

    std::vector<Slot> m_slots;

    std::string m_stopTexture = "slot_test.png";
    std::string m_spinTexture = "slot_test_blur.png";

    float m_startSpeed = 10;

    Vec2 m_slotDimensions;

    bool m_timed = true;
    bool m_spinning = false;
  };
}