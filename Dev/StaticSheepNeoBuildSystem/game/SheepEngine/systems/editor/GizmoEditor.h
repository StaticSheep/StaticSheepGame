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
    // Currently Selected Object
    Handle m_object;
    GameSpace* m_objSpace;

    GizmoType m_mode;

  };
}