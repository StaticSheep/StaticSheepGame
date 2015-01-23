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
#include "components/sprites/CSprite.h"
#include "../graphics/SheepGraphics.h"
#include "components/colliders/CCircleCollider.h"
#include "components/colliders/CBoxCollider.h"

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
  static const int centerSize = pivotRadius * 6;
  static const float arrowSelectProximity = arrowSize * arrowSize;
  static const float pivotSelectProximity = pivotRadius * pivotRadius;

  GizmoEditor::GizmoEditor()
    :m_mode(Scale)
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

  void GizmoEditor::DetectHover(Vec2 screenPos)
  {
    Vec2 mousePos = SHEEPINPUT->Mouse.GetScreenPosition();
    Vec2 topLeft, botRight;

    if ((screenPos - mousePos).SquareLength() < pivotSelectProximity)
      m_pHover = true;
    else
      m_pHover = false;

    if (m_mode != Rotation)
    {
      // Determine the positions of the arrows and if the user is hovering

      m_xArrow = Vec2(screenPos.x + lineLength + arrowSize / 2, screenPos.y);
      topLeft = m_xArrow - 
        Vec2(arrowSize / 2 + lineLength - pivotRadius, arrowSize / 3);

      botRight = m_xArrow - Vec2(arrowSize / 2, -arrowSize / 3);

      if ((m_xArrow - mousePos).SquareLength() < arrowSelectProximity
        || (mousePos > topLeft && botRight > mousePos))
        m_xHover = true;
      else
        m_xHover = false;

      m_yArrow = Vec2(screenPos.x, screenPos.y - lineLength - arrowSize / 2);

      if ((m_yArrow - mousePos).SquareLength() < arrowSelectProximity)
        m_yHover = true;
      else
        m_yHover = false;


    }
    
  }

  void GizmoEditor::DrawHandles(Vec2 screenPos)
  {
    Vec2 mousePos = SHEEPINPUT->Mouse.GetScreenPosition();

    // Draw Pivot
    if (m_pHover || (m_dragging && m_dragAxis == DA_XY))
    {
      Draw::SetColor(1, 1, 0, 1);
    }
    else
    {
      Draw::SetColor(1, 1, 1, 1);
    }
    Draw::SetTexture(pivotID);
    Draw::DrawCircle(screenPos.x, screenPos.y, pivotRadius);
    Draw::DrawRectOutline(screenPos.x - centerSize / 2,
      screenPos.y - centerSize / 2, centerSize, centerSize);


    // Draw Gizmo Hands
    if (m_mode != Rotation)
    {
      // ========================================================  X-axis

      if ((m_dragging && m_dragAxis == DA_X) || m_xHover)
      {
        Draw::SetColor(1, 1, 0, 1);
      }
      else
      {
        Draw::SetColor(1, 0, 0, 1);
      }

      Draw::DrawLine(screenPos.x, screenPos.y,
        screenPos.x + lineLength, screenPos.y);

      if (m_mode == Translation)
      {
        Draw::DrawTriangle(m_xArrow.x - arrowSize / 2,
          m_xArrow.y + arrowSize / 2,
          m_xArrow.x - arrowSize / 2, m_xArrow.y - arrowSize / 2,
          m_xArrow.x + arrowSize / 2, m_xArrow.y);
      }
      else if (m_mode == Scale)
      {
        Draw::DrawQuad(m_xArrow.x - arrowSize / 2, m_xArrow.y + arrowSize / 2,
          arrowSize, arrowSize);
      }


      // ======================================================== Y-axis

      if ((m_dragging && m_dragAxis == DA_Y) || m_yHover)
      {
        Draw::SetColor(1, 1, 0, 1);
      }
      else
      {
        Draw::SetColor(0, 1, 0, 1);
      }

      Draw::DrawLine(screenPos.x, screenPos.y - lineLength,
        screenPos.x, screenPos.y);

      if (m_mode == Translation)
      {
        Draw::DrawTriangle(m_yArrow.x + arrowSize / 2,
          m_yArrow.y + arrowSize / 2,
          m_yArrow.x - arrowSize / 2, m_yArrow.y + arrowSize / 2,
          m_yArrow.x, m_yArrow.y - arrowSize / 2);
      }
      else if (m_mode == Scale)
      {
        Draw::DrawQuad(m_yArrow.x - arrowSize / 2, m_yArrow.y + arrowSize / 2,
          arrowSize, arrowSize);
      }
    }
  }

  void GizmoEditor::DrawSpriteOutline(GameObject* obj, Vec3 objScale,
    Vec2 screenPos, float theta)
  {
    Sprite* spr = obj->GetComponent<Sprite>(eSprite);
    Vec2 spriteSize(spr->Size.x * objScale.x *
      spr->TextureSize.x * m_bronzeRatio.x,
      spr->Size.y * objScale.y * spr->TextureSize.y * m_bronzeRatio.y);

    Draw::SetColor(1.0f, 0.5f, 0.06f, 0.9f);

    Draw::DrawRectOutline(screenPos.x - spriteSize.x  / 2,
      screenPos.y - spriteSize.y / 2,
      spriteSize.x, spriteSize.y, theta);
    
  }

  void GizmoEditor::DrawRigidBodyOutline(GameObject* obj,
    Vec3 objScale, Vec2 screenPos, float theta)
  {
    Draw::SetColor(0.05f, 200 / 255.0f, 1.0f, 0.9f);
    if (obj->HasComponent(eCircleCollider))
    {
      CircleCollider* cc = obj->GetComponent<CircleCollider>(eCircleCollider);
      Draw::DrawCircle(screenPos.x, screenPos.y, cc->m_radius * m_bronzeRatio.x);
    }
    else if (obj->HasComponent(eBoxCollider))
    {
      BoxCollider* bc = obj->GetComponent<BoxCollider>(eBoxCollider);
      Draw::DrawRectOutline(screenPos.x - (bc->m_width * m_bronzeRatio.x) / 2,
        screenPos.y - (bc->m_height * m_bronzeRatio.y) / 2,
        bc->m_width * m_bronzeRatio.x, bc->m_height * m_bronzeRatio.y, theta);
    }
  }

  void GizmoEditor::DetectDrag()
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

  void GizmoEditor::HandleDrag(GameObject* obj)
  {
    Transform* trans = obj->GetComponent<Transform>(eTransform);

    // We are dragging
    if (SHEEPINPUT->Mouse.ButtonReleased(LMB))
    {
      m_dragging = false;
    }
    else
    {
      Vec2 diff = Draw::ToWorld(SHEEPINPUT->Mouse.GetScreenPosition())
        - Draw::ToWorld(m_lastMousePos);

      if (m_mode == Translation)
      {
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
      }
      else if (m_mode == Scale)
      {
        if (m_dragAxis == DA_X)
        {
          trans->SetScale(trans->GetScale() + 
            Vec3(diff.x / m_scaleDampening, 0, 0));
        }
        else if (m_dragAxis == DA_Y)
        {
          trans->SetScale(trans->GetScale()
            + Vec3(0, diff.y / m_scaleDampening, 0));
        }
        else if (m_dragAxis == DA_XY)
        {
          trans->SetScale(trans->GetScale()
            + Vec3(diff.x / m_scaleDampening, diff.y / m_scaleDampening, 0));
        }
      }
      

      

      m_lastMousePos = SHEEPINPUT->Mouse.GetScreenPosition();
    }
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

        auto cam = GRAPHICS->RetrieveCamera(GRAPHICS->GetActiveCamera());

        m_bronzeRatio = Vec2(GRAPHICS->_ScreenWidth / cam->GetScale().x,
          GRAPHICS->_ScreenHeight / cam->GetScale().y);

        Transform* trans = obj->GetComponent<Transform>(eTransform);
        Vec2 screenPos = Draw::ToScreen(trans->GetTranslation());
        float theta = trans->GetRotation();

        DetectHover(screenPos);
        DrawHandles(screenPos);

        if (obj->HasComponent(eSprite))
          DrawSpriteOutline(obj, trans->GetScale(), screenPos, theta);

        DrawRigidBodyOutline(obj, trans->GetScale(), screenPos, theta);
        
        // Selecting / Drag Start
        if (!m_dragging)
        {
          DetectDrag();
        }
        else
        {
          HandleDrag(obj);
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