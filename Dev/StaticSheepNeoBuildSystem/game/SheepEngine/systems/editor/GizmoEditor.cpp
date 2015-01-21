/*****************************************************************
Filename: Editor.cpp
Project:
Author(s): Zachary Nawar (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "systems/editor/GizmoEditor.h"
#include "components/transform/CTransform.h"
#include "Camera/Camera.h"
#include "../input/Input.h"

namespace Framework
{
  DirectSheep::Camera* worldCam = nullptr;
  GizmoEditor* GIZMO_EDITOR = nullptr;

  
  static unsigned pivotID = -1;
  static unsigned gizmoTipID = -1;

  GizmoEditor::GizmoEditor()
    :m_mode(Translation)
  {

  }

  GizmoEditor::~GizmoEditor()
  {

  }

  void GizmoEditor::Initialize()
  {
    pivotID = Draw::GetTextureID("gizmo_pivot.png");
    gizmoTipID = Draw::GetTextureID("gizmo_tip.png");
  }

  void GizmoEditor::Update(float dt)
  {
    if (ENGINE->m_editorAcitve)
    {
      if (m_object != Handle::null)
      {

      }
    }
    else
    {

    }
  }

  void GizmoEditor::SetSelectedObject(GameObject* obj)
  {
    if (obj == nullptr)
    {
      m_object = Handle::null;
      m_objSpace = nullptr;
    }
    else
    {
      m_object = obj->self;
      m_objSpace = obj->space;
    }   
  }

  GizmoType GizmoEditor::GetGizmoMode()
  {
    return m_mode;
  }

  void GizmoEditor::SetGizmoMode(GizmoType mode)
  {
    m_mode = mode;
  }

  static const int lineWidth = 4;
  static const int arrowSize = 16;
  static const int lineLength = 60;
  static const float selectProximity = 16 * 16;

  void GizmoEditor::Draw()
  {
    if (m_object != Handle::null)
    {
      GameObject* obj = m_objSpace->GetGameObject(m_object);
      Transform* trans = obj->GetComponent<Transform>(eTransform);
      
      Vec2 screenPos = Draw::ToScreen(trans->GetTranslation());
      Vec2 mousePos = SHEEPINPUT->Mouse.GetScreenPosition();
      Vec2 arrowPos;

      // Draw Pivot
      Draw::SetColor(255, 255, 255, 255);
      Draw::SetTexture(pivotID);
      Draw::DrawTexturedRect(screenPos.x - 10, screenPos.y - 10, 20, 20);

      // Draw Gizmo Hands
      if (m_mode == Translation)
      {
        // X-axis
        Draw::SetColor(1, 0, 0, 1);
        Draw::DrawLine(screenPos.x, screenPos.y,
          screenPos.x + lineLength, screenPos.y, lineWidth);
        
        Draw::SetTexture(gizmoTipID);
        arrowPos = Vec2(screenPos.x + lineLength + arrowSize / 2, screenPos.y);
        if ((arrowPos - mousePos).SquareLength() < selectProximity)
          Draw::SetColor(1, 1, 0, 1);
        else
          Draw::SetColor(1, 0, 0, 0.8f);

        Draw::DrawTexturedRectRotated(screenPos.x + lineLength,
          screenPos.y - arrowSize / 2,
          arrowSize, arrowSize, -PI / 2);
        

        // Y-axis
        Draw::SetColor(0, 1, 0, 1);
        Draw::DrawLine(screenPos.x, screenPos.y - lineLength,
          screenPos.x, screenPos.y, lineWidth);

        Draw::DrawRect(800, 450, 6, 6);

        Draw::DrawLine(100, 400, 100, 600, 4);

        Draw::DrawRect(100, 600, 6, 6);

        Draw::SetTexture(gizmoTipID);
        arrowPos = Vec2(screenPos.x, screenPos.y - lineLength - arrowSize / 2);
        if ((arrowPos - mousePos).SquareLength() < selectProximity)
          Draw::SetColor(1, 1, 0, 1);
        else
          Draw::SetColor(0, 1, 0, 0.8f);

        Draw::DrawTexturedRectRotated(screenPos.x - arrowSize / 2,
          screenPos.y - lineLength - arrowSize, arrowSize, arrowSize, 0);
        
      }
    }

  }

  void GizmoEditor::ReceiveMessage(Message& msg)
  {
    ObjectSelectedMessage* objectMessage;
    switch (msg.MessageId)
    {
    case Message::ObjectSelected:
      objectMessage = (ObjectSelectedMessage*)&msg;
      SetSelectedObject(objectMessage->obj);
      break;
    case Message::GUIDraw:
      Draw();
      break;
    default:
      break;
    }
  }
}