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
    void TweakSetupSlots(const void* value);

    void Update(float dt);
    void Draw();

    void Start();
    void Stop();

    bool startSpinning = true;

    int slotOptions = 3;
    int numSlots = 3;

    float startSpeed = 7;

    Vec2 slotOffset;
    Vec2 slotSize = Vec2(1, 1);
    float slotMargin = 10;

    float startTime = 3.0f;
    float bonusTime = 0.2f;

    Vec2 slotBackSize = Vec2(1, 1);
    
    std::string m_stopTexture = "ggg_reel.png";
    std::string m_spinTexture = "ggg_reel_blur.png";
    std::string m_slotBackTexture = "slotmachine_slotback.png";

    Vec4 backingColor = Vec4(1, 1, 1, 1);
    bool useSpriteColor = true;

    // (int slotNum, int* spinID, int* stopID)
    void SetTextureCB(Handle obj, Function cb);
    // (int slotNum, int* result)
    void SetSelectionCB(Handle obj, Function cb);
    // (std::vector<int>* results)
    void SetFinishedCB(Handle obj, Function cb);

    void TestFunction(int slotNum, int* spinID, int* stopID);

    int m_layer = 1;
    bool m_hooked = false;

    int m_order = 0;
  private:

    void SetupSlots();
    void SlotsFinished();

    // (int slotNum, int* spinID, int* stopID)
    Function customReelCB;
    Handle customReelCBObj;
    // (int slotNum, int* result)
    Function selectionCB;
    Handle selectionCBObj;
    // (std::vector<int>* results)
    Function finishCB;
    Handle finishCBObj;


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

    int m_numSpinning = 0;

    std::vector<Slot> m_slots;
    std::vector<int> m_results;
    
    int m_slotbackTextureID = -1;

    float m_startSpeed = 10;

    Vec2 m_slotDimensions;
    Vec2 m_slotBackDimensions;

    bool m_timed = true;
    bool m_spinning = false;
  };
}