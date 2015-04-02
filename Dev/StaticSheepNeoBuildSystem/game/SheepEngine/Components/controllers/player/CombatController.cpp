/*****************************************************************
Filename: CombatController.cpp
Project:
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "pch/precompiled.h"
#include "CombatController.h"
#include "CPlayerController.h"
#include "../../gameplay_scripts/Weapon_Scripts/CBullet_default.h"
#include "../../gameplay_scripts/Level_Scripts/CGrinder.h"
#include "../../gameplay_scripts/Level_Event_Scripts/CAsteroid.h"

#define SHIELD_FADETIME 3.0f
#define SHIELD_HOLDTIME 3.0f
#define SHIELD_DANGERFLICKER 0.1f
#define SHIELD_FLASHTIME 0.3f



namespace Framework
{
  LightColor CombatController::FullShieldFadeColor
    = LightColor(0.1f, 0.9f, 1.0f, 0.1f);

  LightColor CombatController::FullShieldColor
    = LightColor(0.0f, 0.9f, 1.0f, 1.0f);

  /* Color of shields when they are just above empty */
  LightColor CombatController::LowShieldColor
    = LightColor(1.0f, 0.1f, 0.0f, 0.95f);
    
  LightColor CombatController::MedShieldColor
    = LightColor(1.0f, 1.0f, 0.1f, 0.95f);

  CombatController::CombatController()
  {

  }

  CombatController::~CombatController()
  {

  }

  //void CombatController::Initialize(GameObject* obj, int playerNum)
  //{
  //  m_playerNum = playerNum;

  //}

  void CombatController::Initialize(GameObject* obj, int playerNum)
  {
    m_playerNum = playerNum;
    //m_playerController = pc->self;
    m_spineLight = obj->GetComponentHandle(eSpineLight);
    m_space = obj->space;

    m_owner = obj->self;

    SetHealth(m_maxHealth);
    SetShields(m_maxShields);

    //TakeDamage(1.0f, -1);
  }

  void CombatController::Collision(GameObject* obj, GameObject* OtherObject)
  {
    Bullet_Default* bullet = OtherObject->GetComponent
      <Bullet_Default>(eBullet_Default);

    SoundEmitter* se = obj->GetComponent
      <SoundEmitter>(eSoundEmitter);

    Transform* pt = obj->GetComponent
      <Transform>(eTransform);



    if (bullet)
    {
      /* Inflict the damage to the player (or atleast attempt) */
      TakeDamage(bullet->damage, bullet->playerOwner);

      /* Play hit sound */
      se->Play("hit1", &SoundInstance(1.0f));

      /* Hit effect */
      float randomX = (float)GetRandom(-25, 25);
      float randomY = (float)GetRandom(-25, 25);
      Handle hitHandle = (FACTORY->LoadObjectFromArchetype(m_space, "hit"))->self;

      /* Set the translation of the hit effect. */
      m_space->GetGameObject(hitHandle)->GetComponent<Transform>(eTransform)
        ->SetTranslation(pt->GetTranslation() + Vec3(randomX, randomY, -1.0f));

      return;
    }

    /* We collided with a grinder, woo 
    @NOTE: This code should be moved to grinder component code? */
    if (OtherObject->GetComponent<Grinder>(eGrinder))
    {
      TakeDamage(5, -1);
      return;
    }

    if (OtherObject->GetComponent<Asteroid>(eAsteroid))
    {
      TakeDamage(50, -1);
      // @TODO: Add random push?
      return;
    }
  }

  void CombatController::Update(GameObject* obj, float dt)
  {

    /* Player is dead, kill them */
    if (m_dead)
    {
      TrueDeath();
      return;
    }

    /* ==== ==== ==== Regenerate shields ==== ==== ==== */
    if (m_rechargingShields)
    {
      if (m_nextRegen < 0.0f)
      {
        if (m_shield < m_maxShields)
        {
          m_shield += m_regenRate * dt;

          /* Cap out at max shields of course */
          if (m_shield > m_maxShields)
          {
            m_shield = m_maxShields;
            ShieldsRecharged(obj);
            obj->hooks.Call("ShieldsRecharged");
          }

        }
      }
      else
      {
        m_nextRegen -= dt;

        if (m_nextRegen < 0.0f)
        {
          ShieldsRecharging(obj);
          obj->hooks.Call("ShieldsRecharging");
        }
      }
    } /* ==== ==== ==== ==== ==== ==== ==== ==== ==== ==== */
    else 
    { 
      /* ==== ==== ==== Fading shields ==== ==== ==== */
      if (m_fadeTime > 0.0f)
      {
        m_fadeTime -= dt;
        if (m_fadeTime < 0.0f)
          m_fadeTime = 0.0f;
      }
    }

    /* ==== ==== Respawn god mode ==== ==== */
    if (m_respawnGodTime > 0.0f)
    {
      m_respawnGodTime -= dt;

      if (m_respawnGodTime < 0.0f)
        m_respawnGod = false;
    }

    UpdateShieldLight(obj, dt);
  }

  void CombatController::TakeDamage(float damage,
    int attacker, bool explosive, bool penetrate)
  {
    /* God cannot be injured */
    if (m_godMode || m_respawnGod)
      return;
    
    /* Thou shall not injure thouself */
    if (attacker == m_playerNum && !explosive)
      return;

    /* Thou can not be healed by damage */
    if (damage < 0.0f)
      return;

    float realDamage = damage;

    if (penetrate)
    {
      m_health -= damage;
    }
    else
    {
      /* Tells the shields they were damaged and to charged */
      m_rechargingShields = true;

      /* If we have shields then shields absorb
      the damage first */
      if (m_shield > 0.0f)
      {
        /* Hit the shields */
        m_shield -= damage;

        if (m_shield < 0.0f)
        {
          /* If the damage was too much for the shield
          then take away the absorbed damage*/
          damage = -m_shield;
          m_shield = 0.0f;

          /* Takes twice as long to regen if you hit zero */
          m_nextRegen = m_regenCD * 2;
        }
        else
        {
          m_shieldFlash = SHIELD_FLASHTIME;
          m_nextRegen = m_regenCD;
          damage = 0;
        }
      }

      /* Any remaining damage goes to health */
      if (damage > 0.0f)
        m_health -= damage;
    }

    TRACELOG->Log(TraceLevel::DBG,
      "Player %d took %.2f damage from player %d. [HP: %.2f/%.2f | SH: %.2f/%.2f]",
      m_playerNum, realDamage, attacker, m_health,
      m_maxHealth, m_shield, m_maxShields);

    /* You are dead. */
    if (m_health <= 0.0f)
      Kill(attacker);

  }

  void CombatController::Kill(int attacker)
  {
    if (m_dead)
      return;

    m_health = 0.0f;
    m_shield = 0.0f;

    TRACELOG->Log(TraceLevel::DBG, "Player %d has been killed by player %d",
      m_playerNum, attacker);

    m_dead = true;
    m_killer = attacker;
  }

  void CombatController::TrueDeath()
  {
    m_space->GetGameObject(m_owner)->hooks.Call("PlayerKilled", m_killer);

    m_space->hooks.Call("PlayerDied", m_playerNum, m_killer);

    MetricInfo metricData(m_killer, 0, 0,
      PLAYER_KILL, Buttons::NONE, Weapons::PISTOL);

    ENGINE->SystemMessage(MetricsMessage(&metricData));
  }

  void CombatController::ShieldsRecharging(GameObject* obj)
  {
    SoundEmitter* se = obj->GetComponent<SoundEmitter>(eSoundEmitter);

    if (!se)
      return;

    se->Play(m_rechargingSound);
  }

  void CombatController::ShieldsRecharged(GameObject* obj)
  {
    SoundEmitter* se = obj->GetComponent<SoundEmitter>(eSoundEmitter);

    if (se)
      se->Play(m_rechargedSound);

    m_fadeTime = SHIELD_FADETIME + SHIELD_HOLDTIME;
    m_rechargingShields = false;
  }

  void CombatController::UpdateShieldLight(GameObject* obj, float dt)
  {
    SpineLight* sl = obj->GetComponent<SpineLight>(eSpineLight);

    if (!sl)
      return;

    if (m_rechargingShields)
    {
      if (m_shield == 0)
      {
        m_nextDangerSwitch -= dt;

        /* We have absolutely no shields */
        if (m_nextDangerSwitch <= 0.0f)
        {
          sl->SetColor(m_dangerColor);
          sl->Toggle(!sl->GetStatus());
          m_nextDangerSwitch = SHIELD_DANGERFLICKER;
          m_shieldFlash = 0.0f;
        }
      }
      else
      {
        if (m_shieldFlash <= 0.0f)
        {
          /* Shields are recharging or holding still */
          LightColor curColor = Ease::TwoStepLinear(m_shield / m_maxShields,
            LowShieldColor, MedShieldColor, FullShieldColor);

          sl->Toggle(true);
          sl->SetColor(curColor);
        }
        else
        {
          /* Shields were just hit, flash a color */
          m_shieldFlash -= dt;

          LightColor curColor = LightColor(0.6f, 0.9f, 1.0f, 0.9f);

          sl->Toggle(true);
          sl->SetColor(curColor);
        }
        
      }
    }
    else
    {
      if (m_fadeTime > 0.0f)
      {
        m_fadeTime -= dt;

        if (m_fadeTime < 0.0f)
          m_fadeTime = 0.0f;

        if (m_fadeTime < SHIELD_FADETIME)
        {
          LightColor curColor = Ease::Linear(m_fadeTime / SHIELD_FADETIME,
            FullShieldFadeColor, FullShieldColor);

          sl->SetColor(curColor);

          if (m_fadeTime <= 0.0f)
          {
            //sl->SetColor(LightColor(1.0f, 1.0f, 1.0f, 0.0f));
            sl->Toggle(false);
          } 
        }
      }
      
      
    }



  }


}