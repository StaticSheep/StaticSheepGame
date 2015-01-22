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

  private:

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
    bool m_pHover = false;

    Vec2 m_lastMousePos;
    

    GizmoType m_mode = Translation;

  };
}