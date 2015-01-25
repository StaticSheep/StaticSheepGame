/*****************************************************************
Filename: Editor.h
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
#pragma once

#include "systems/System.h"
#include "types/object/Object.h"

namespace Framework
{

  enum GizmoType
  {
    Translation,
    Scale,
    Rotation
  };

  class GizmoEditor;
  
  extern GizmoEditor* GIZMO_EDITOR;
  
  class GizmoEditor : public ISystem
  {
  public:
    GizmoEditor();
    ~GizmoEditor();

    // Initialization call for the system
    virtual void Initialize();

    // All system can be updated every frame through this call
    virtual void Update(float dt);

    
    void Draw();

    // Systems receive and handle messages in different ways
    virtual void ReceiveMessage(Message& msg);

    // Systems can have names too!
    virtual std::string GetName() { return std::string("Editor"); };

    void SetSelectedObject(GameObject* obj);

    void SetGizmoMode(GizmoType mode);
    GizmoType GetGizmoMode();

    void ToggleSnapping(bool on);

  private:

    void DetectHover(Vec2 screenPos, float theta);
    void DrawHandles(Vec2 screenPos, float theta);

    void DrawSpriteOutline(GameObject* obj, Vec3 objScale, Vec2 screenPos,
      float theta);
    void DrawRigidBodyOutline(GameObject* obj, Vec3 objScale, Vec2 screenPos,
      float theta);

    void DetectDrag();
    void HandleDrag(Vec2 screenPos, GameObject* obj);

    enum DragAxis
    {
      DA_None,
      DA_X,
      DA_Y,
      DA_XY,
    };
    // Currently Selected Object
    Handle m_object;
    GameSpace* m_objSpace;

    bool m_dragging = false;
    DragAxis m_dragAxis = DA_None;

    bool m_xHover = false;
    bool m_yHover = false;
    bool m_rHover = false;
    bool m_pHover = false;

    Vec2 m_bronzeRatio;

    Vec2 m_lastMousePos;
    Vec2 m_startRotatePos;
    Vec2 m_xArrow, m_xArrowCenter;
    Vec2 m_yArrow, m_yArrowCenter;

    Vec2 m_snapBuffer = 0;
    
    float m_scaleDampening = 25.0f;
    float m_snapDistance = 50.0f;
    bool m_useSnapping = false;
    bool m_world = true;

    GizmoType m_mode = Translation;

    friend class Engine;
  };
}