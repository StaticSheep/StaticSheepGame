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

  static const int lineWidth = 4;
  static const int arrowSize = 16;
  static const int lineLength = 60;
  static const int pivotRadius = 8;
  static const float arrowSelectProximity = arrowSize * arrowSize;
  static const float pivotSelectProximity = pivotRadius * pivotRadius;

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
        GameObject* obj = m_objSpace->GetGameObject(m_object);
        if (obj == nullptr)
        {
          m_object = Handle::null;
          return;
        }
        Transform* trans = obj->GetComponent<Transform>(eTransform);

        Vec2 screenPos = Draw::ToScreen(trans->GetTranslation());
        Vec2 mousePos = SHEEPINPUT->Mouse.GetScreenPosition();
        Vec2 arrowPos;

        // Draw Pivot
        if ((screenPos - mousePos).SquareLength() < pivotSelectProximity ||
          (m_dragging && m_dragAxis == DA_XY))
        {
          Draw::SetColor(1, 1, 0, 1);
          m_pHover = true;
        }
        else
        {
          Draw::SetColor(1, 1, 1, 1);
          m_pHover = false;
        }
        Draw::SetTexture(pivotID);
        Draw::DrawCircle(screenPos.x, screenPos.y, pivotRadius);

        

        // Draw Gizmo Hands
        if (m_mode == Translation)
        {
          // X-axis
          arrowPos = Vec2(screenPos.x + lineLength + arrowSize / 2, screenPos.y);
          if ((arrowPos - mousePos).SquareLength() < arrowSelectProximity
            || (m_dragging && m_dragAxis == DA_X))
          {
            Draw::SetColor(1, 1, 0, 1);
            m_xHover = true;
          }
          else
          {
            Draw::SetColor(1, 0, 0, 1);
            m_xHover = false;
          }
          Draw::DrawLine(screenPos.x, screenPos.y,
            screenPos.x + lineLength, screenPos.y);
          Draw::DrawTriangle(arrowPos.x - arrowSize / 2, arrowPos.y + arrowSize / 2,
            arrowPos.x - arrowSize / 2, arrowPos.y - arrowSize / 2,
            arrowPos.x + arrowSize / 2, arrowPos.y);


          // Y-axis
          arrowPos = Vec2(screenPos.x, screenPos.y - lineLength - arrowSize / 2);
          if ((arrowPos - mousePos).SquareLength() < arrowSelectProximity)
          {
            Draw::SetColor(1, 1, 0, 1);
            m_yHover = true;
          }
          else
          {
            Draw::SetColor(0, 1, 0, 1);
            m_yHover = false;
          }
          Draw::DrawLine(screenPos.x, screenPos.y - lineLength,
            screenPos.x, screenPos.y);
          Draw::DrawTriangle(arrowPos.x + arrowSize / 2, arrowPos.y + arrowSize / 2,
            arrowPos.x - arrowSize / 2, arrowPos.y + arrowSize / 2,
            arrowPos.x, arrowPos.y - arrowSize / 2);
        }


        // Selecting / Drag Start
        if (!m_dragging)
        {
          if (SHEEPINPUT->Mouse.ButtonDown(LMB))
          {
            if (m_pHover)
            {
              m_dragging = true;
              m_dragAxis = DA_XY;
            }
            else if (m_xHover)
            {
              m_dragging = true;
              m_dragAxis = DA_X;
            }
            else if (m_yHover)
            {
              m_dragging = true;
              m_dragAxis = DA_Y;
            }

            m_lastMousePos = SHEEPINPUT->Mouse.GetScreenPosition();
          }
        }
        else
        {
          // We are dragging
          if (SHEEPINPUT->Mouse.ButtonReleased(LMB))
          {
            m_dragging = false;
          }
          else
          {
            Vec2 diff = Draw::ToWorld(SHEEPINPUT->Mouse.GetScreenPosition()) - Draw::ToWorld(m_lastMousePos);
            if (m_dragAxis == DA_X)
            {
              trans->SetTranslation(trans->GetTranslation() + Vec3(diff.x, 0, 0));
            }
            else if (m_dragAxis == DA_Y)
            {
              trans->SetTranslation(trans->GetTranslation() + Vec3(0, diff.y, 0));
            }
            else if (m_dragAxis == DA_XY)
            {
              trans->SetTranslation(trans->GetTranslation() + Vec3(diff.x, diff.y, 0));
            }

            m_lastMousePos = SHEEPINPUT->Mouse.GetScreenPosition();
          }
        }


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

  

  void GizmoEditor::Draw()
  {
    

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