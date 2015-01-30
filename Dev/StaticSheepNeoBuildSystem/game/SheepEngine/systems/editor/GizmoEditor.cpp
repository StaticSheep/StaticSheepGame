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
#include "../anttweak/AntTweakModule.h"

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
  static const float rotationRadius = 140;
  static const int centerSize = pivotRadius * 6;
  static const float arrowSelectProximity = arrowSize * arrowSize;
  static const float pivotSelectProximity = pivotRadius * pivotRadius;

  static const float rotationProximityBuffer = 4;

  static const float rotationMinSelectProximity =
    (rotationRadius - rotationProximityBuffer) *
    (rotationRadius - rotationProximityBuffer);

  static const float rotationMaxSelectProximity =
    (rotationRadius + rotationProximityBuffer) *
    (rotationRadius + rotationProximityBuffer);

  GizmoEditor::GizmoEditor()
    :m_mode(Translation)
  {
    GIZMO_EDITOR = this;
  }

  GizmoEditor::~GizmoEditor()
  {

  }

  void GizmoEditor::Initialize()
  {
    pivotID = Draw::GetTextureID("gizmo_pivot.png");
    gizmoTipID = Draw::GetTextureID("gizmo_tip.png");
  }

  void GizmoEditor::DetectHover(Vec2 screenPos, float theta)
  {
    Vec2 mousePos = SHEEPINPUT->Mouse.GetScreenPosition();
    Vec2 lineStart;

    if ((screenPos - mousePos).SquareLength() < pivotSelectProximity)
      m_pHover = true;
    else
      m_pHover = false;

    if (m_mode != Rotation)
    {
      // Determine the positions of the arrows and if the user is hovering
      if (m_world)
        theta = 0;

      m_xArrow = Vec2(screenPos.x + lineLength + arrowSize / 2, screenPos.y);

      lineStart = screenPos + Vec2(pivotRadius * 2 * cos(theta),
        pivotRadius * 2 * sin(theta));
      m_xArrow = screenPos + Vec2(lineLength * cos(theta),
        lineLength * sin(theta));
      m_xArrowCenter = screenPos + Vec2((lineLength + arrowSize / 2)
        * cos(theta),
        (lineLength + arrowSize / 2) * sin(theta));


      if ((m_xArrow - mousePos).SquareLength() < arrowSelectProximity
        || DistanceFromSegment(lineStart, m_xArrow, mousePos) < pivotRadius
        )
        m_xHover = true;
      else
        m_xHover = false;

      lineStart = screenPos + Vec2(pivotRadius * 2 * cos(theta - PI / 2),
        pivotRadius * 2 * sin(theta - PI / 2));

      m_yArrow = screenPos + Vec2(lineLength * cos(theta - PI / 2),
        lineLength * sin(theta - PI / 2));

      if ((m_yArrow - mousePos).SquareLength() < arrowSelectProximity
        || DistanceFromSegment(lineStart, m_yArrow, mousePos) < pivotRadius)
        m_yHover = true;
      else
        m_yHover = false;


    }
    else
    {
      // Rotation
      if ((screenPos - mousePos).SquareLength() < rotationMaxSelectProximity
        && (screenPos - mousePos).SquareLength() > rotationMinSelectProximity)
      {
        if (!m_dragging)
          m_startRotatePos = mousePos;

        m_rHover = true;
      }
      else
      {
        m_rHover = false;
      }
      
    }
    
  }

  void GizmoEditor::DrawHandles(Vec2 screenPos, float theta)
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

    // Draw Gizmo Hands
    if (m_mode != Rotation)
    {
      Draw::DrawRectOutline(screenPos.x - centerSize / 2,
        screenPos.y - centerSize / 2, centerSize, centerSize);

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
        m_xArrow.x, m_xArrow.y);

      if (m_mode == Translation)
      {
        Vec2 t1(-arrowSize / 2, -arrowSize / 2),
          t2(arrowSize / 2, 0),
          t3(-arrowSize / 2, arrowSize / 2);

        if (!m_world)
        {
          t1 = Draw::RotateAroundPoint(t1, Vec3(), theta);
          t2 = Draw::RotateAroundPoint(t2, Vec3(), theta);
          t3 = Draw::RotateAroundPoint(t3, Vec3(), theta);
        }
        
        Draw::DrawTriangle(t1.x + m_xArrow.x, t1.y + m_xArrow.y,
          t2.x + m_xArrow.x, t2.y + m_xArrow.y,
          t3.x + m_xArrow.x, t3.y + m_xArrow.y);
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

      Draw::DrawLine(screenPos.x, screenPos.y,
        m_yArrow.x, m_yArrow.y);

      if (m_mode == Translation)
      {
        Vec2 t1(0, -arrowSize / 2),
          t2(arrowSize / 2, arrowSize / 2),
          t3(-arrowSize / 2, arrowSize / 2);

        if (!m_world)
        {
          t1 = Draw::RotateAroundPoint(t1, Vec3(), theta);
          t2 = Draw::RotateAroundPoint(t2, Vec3(), theta);
          t3 = Draw::RotateAroundPoint(t3, Vec3(), theta);
        }

        Draw::DrawTriangle(t1.x + m_yArrow.x, t1.y + m_yArrow.y,
          t2.x + m_yArrow.x, t2.y + m_yArrow.y,
          t3.x + m_yArrow.x, t3.y + m_yArrow.y);

      }
      else if (m_mode == Scale)
      {
        Draw::DrawQuad(m_yArrow.x - arrowSize / 2, m_yArrow.y + arrowSize / 2,
          arrowSize, arrowSize);
      }
    }
    else
    {
      // Rotation
      if (m_rHover || m_dragging)
      {
        Draw::SetColor(1, 1, 0, 1);
      }
      else
      {
        Draw::SetColor(1, 1, 1, 1);
      }

      Draw::DrawCircle(screenPos.x, screenPos.y, rotationRadius);

      

      if (m_rHover || m_dragging)
      {
        Vec2 dir = (screenPos - m_startRotatePos).Normalize(); 
        Vec2 start = screenPos - dir * rotationRadius;
        Vec2 end = start + Vec2(dir.y, -dir.x) * 110;
        start = start + Vec2(dir.y, -dir.x) * -110;

        Draw::SetColor(1, 0, 0, 1);
        Draw::DrawLine(start.x, start.y, end.x, end.y);
      }
      if (m_dragging)
      {
        Vec2 start, end;
        Vec2 dir = Vec2(cos(theta), sin(theta));
        start = screenPos - dir * rotationRadius;
        end = screenPos + dir * rotationRadius;

        Draw::SetColor(1, 0, 0, 1);
        Draw::DrawLine(start.x, start.y, end.x, end.y);

        start = screenPos - Vec2(dir.y, -dir.x) * rotationRadius;
        end = screenPos + Vec2(dir.y, -dir.x) * rotationRadius;

        Draw::SetColor(0, 1, 0, 1);
        Draw::DrawLine(start.x, start.y, end.x, end.y);
      }
      
    }
  }

  void GizmoEditor::DrawSpriteOutline(GameObject* obj, Vec3 objScale,
    Vec2 screenPos, float theta)
  {
    Sprite* spr = obj->GetComponent<Sprite>(eSprite);
    float ratio = 100.0f / DirectSheep::Camera::perspectiveW;
    Vec2 spriteSize(spr->Size.x * objScale.x *
      spr->TextureSize.x * m_bronzeRatio.x,
      spr->Size.y * objScale.y * spr->TextureSize.y * m_bronzeRatio.y);
    spriteSize = ratio * spriteSize;

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
      else if (m_rHover)
      {
        m_dragging = true;
      }

      m_lastMousePos = SHEEPINPUT->Mouse.GetScreenPosition();
    }
  }

  void GizmoEditor::HandleDrag(Vec2 screenPos, GameObject* obj)
  {
    Transform* trans = obj->GetComponent<Transform>(eTransform);
    float theta = trans->GetRotation();


    // We are dragging
    if (SHEEPINPUT->Mouse.ButtonReleased(LMB))
    {
      m_dragging = false;
    }
    else
    {
      Vec2 diff = Draw::ToWorld(SHEEPINPUT->Mouse.GetScreenPosition())
        - Draw::ToWorld(m_lastMousePos);


      if (m_useSnapping)
      {
        int reps;

        m_snapBuffer += diff;

        reps = (int)(m_snapBuffer.x / m_snapDistance);
        diff.x = reps * m_snapDistance;
        m_snapBuffer.x -= reps * m_snapDistance;

        reps = (int)(m_snapBuffer.y / m_snapDistance);
        diff.y = reps * m_snapDistance;
        m_snapBuffer.y -= reps * m_snapDistance;

        if (m_mode == Rotation)
        {
          m_snapRotateBuffer += diff.Length();
          reps = (int)(m_snapRotateBuffer / m_snapDistance);
          diff.Normalize();
          diff = diff * reps * m_snapDistance;
          m_snapRotateBuffer -= reps * m_snapDistance;
        }
        
      }
      else
      {
        m_snapBuffer = 0;
        m_snapRotateBuffer = 0;
      }
        

      if (m_mode == Translation)
      {
        if (m_dragAxis == DA_X)
        {
          if (m_world)
          {
            trans->SetTranslation(trans->GetTranslation()
              + Vec3(diff.x, 0, 0));
          }
          else
          {

          }
          
        }
        else if (m_dragAxis == DA_Y)
        {
          if (m_world)
          {
            trans->SetTranslation(trans->GetTranslation()
              + Vec3(0, diff.y, 0));
          }
          else
          {

          }
        }
        else if (m_dragAxis == DA_XY)
        {
          trans->SetTranslation(trans->GetTranslation()
            + Vec3(diff.x, diff.y, 0));
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
          Vec3 scale = trans->GetScale();
          float xymod = scale.x / scale.y;
          trans->SetScale(scale
            + Vec3((diff.x + diff.y) / m_scaleDampening * xymod,
            (diff.x + diff.y) / m_scaleDampening, 0));
        }
      }
      else if (m_mode == Rotation)
      {
        Vec2 dir = (screenPos - m_startRotatePos).Normalize();
        //dir = Vec2(dir.y, -dir.x);

        if (diff * dir >= 0)
          trans->SetRotation(trans->GetRotation() + (PI / 180) * diff.Length());
        else
          trans->SetRotation(trans->GetRotation() - (PI / 180) * diff.Length());
      }
      

      

      m_lastMousePos = SHEEPINPUT->Mouse.GetScreenPosition();
    }
  }

  void GizmoEditor::DetectSelect()
  {
    Vec2 mousePos = SHEEPINPUT->Mouse.GetScreenPosition();
    if (!SHEEPINPUT->Mouse.ButtonPressed(RMB))
      return;

    if (ENGINE->Spaces().size() == 0)
      return;


    if (m_dragging)
      return;

    GameSpace* sp;
    GameObject* obj;
    Transform* tr;
    Sprite* spr;
    std::vector<Transform*> good_objects;
    Vec2 topleft;
    Vec2 botRight;
    float ratio;

    for (int s = 0; s < ENGINE->Spaces().size(); ++s)
    {
      sp = ENGINE->Spaces()[s];
      for (int i = 0; i < sp->m_objects.Size(); ++i)
      {
        obj = (GameObject*)sp->m_objects[i];
        tr = obj->GetComponent<Transform>(eTransform);
        spr = obj->GetComponent<Sprite>(eSprite);

        if (spr)
        {
          topleft = Draw::ToScreen(tr->GetTranslation());

          ratio = 100.0f / DirectSheep::Camera::perspectiveW;
          topleft.x -= tr->GetScale().x * spr->TextureSize.x * spr->Size.x * ratio * 0.5f;
          topleft.y -= tr->GetScale().y * spr->TextureSize.y * spr->Size.y * ratio * 0.5f;
          botRight = topleft;
          botRight.x += tr->GetScale().x * spr->TextureSize.x * spr->Size.x * ratio;
          botRight.y += tr->GetScale().y * spr->TextureSize.y * spr->Size.y * ratio;

          if (mousePos > topleft && mousePos < botRight)
          {
            good_objects.push_back(tr);
          }

        }
      }
    }


    float bestZ = 1000.0f;
    Transform* cur = nullptr;
    Transform* picked = nullptr;
    for (int i = 0; i < good_objects.size(); ++i)
    {
      cur = good_objects[i];
      if (cur->GetTranslation().z < bestZ)
      {
        picked = cur;
        bestZ = cur->GetTranslation().z;
      }
    }
    if (picked)
    {
      obj = picked->space->GetGameObject(picked->owner);
      obj->TweakObject();
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
        float theta = -trans->GetRotation();

        DetectHover(screenPos, theta);
        DrawHandles(screenPos, theta);

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
          HandleDrag(screenPos, obj);
        }
          


      }
      else
      {
        
      }

      if (!m_dragging)
        DetectSelect();

    }
    else
    {

    }
  }

  void GizmoEditor::SetSelectedObject(GameObject* obj)
  {
    if (obj == nullptr)
    {
      GameObject* oldObject = m_objSpace->GetGameObject(m_object);
      if (oldObject)
        oldObject->m_editorFocus = false;

      m_object = Handle::null;
      m_objSpace = nullptr;
    }
    else
    {
      if (m_objSpace)
      {
        GameObject* oldObject = m_objSpace->GetGameObject(m_object);
        if (oldObject)
          oldObject->m_editorFocus = false;
      }

      obj->m_editorFocus = true;
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

  void GizmoEditor::HighlightBar(GameObject* obj)
  {
    auto spaces = ENGINE->Spaces();
    for (int i = 0; i < spaces.size(); ++i)
    {
      for (int j = 0; j < spaces[i]->m_objects.Size(); ++j)
      {
        GameObject* robj = (GameObject*)(spaces[i]->m_objects[j]);
        AntTweak::TBar* bar = ATWEAK->GetBar(robj->tweakHandle);
        if (!bar)
          continue;

        if (m_oneObjectBar)
        {
          if (obj != robj)
          {
            ATWEAK->RemoveBar(bar);
            robj->tweakHandle = Handle::null;
          }
          else
            bar->SetColor(180, 200, 0);
        }
        else
        {
          if (obj != robj)
            bar->SetColor(0, 0, 100);
          else
            bar->SetColor(180, 200, 0);
        }
        
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
      HighlightBar(objectMessage->obj);
      break;
    case Message::GUIDraw:
      Draw();
      break;
    default:
      break;
    }
  }
}