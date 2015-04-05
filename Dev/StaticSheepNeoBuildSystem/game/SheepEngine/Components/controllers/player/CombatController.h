/*****************************************************************
Filename: CombatController.h
Project:
Author(s): Zachary Nawar (Primary)

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#pragma once

#include "types\space\Space.h"

namespace Framework
{
  //class PlayerController;

  class CombatController
  {
  public:
    CombatController();
    ~CombatController();

    void Initialize(GameObject* obj, int playerNum);
    //void Initialize(PlayerController* pc);

    /* Called once per frame */
    void Update(GameObject* obj, float dt);
    
    /* Called upon colliding with another object */
    void Collision(GameObject* obj, GameObject* OtherObject);

    /* Called when taking damage from a source */
    void TakeDamage(float damage, int attacker, bool explosive = false,
      bool penetrates = false);

    /* Call to kill */
    void Kill(int attacker);

    /* Used for setting the max amount of shields / health */
    void SetMaxHealth(float maxHealth);
    void SetMaxShields(float maxShield);

    /* Adds (or subtracts if negative) to shields / HP */
    void AddHealth(float amount) { m_health += amount; }
    void AddShields(float amount) { m_shield += amount; }

    /* Sets the current health / Shield*/
    void SetHealth(float amount) { m_health = amount; }
    void SetShields(float amount)
    { m_shield = amount; m_rechargingShields = true; }

    /* Gets the current shield / health */
    float GetHealth() { return m_health; }
    float GetShields() { return m_shield; }

    /* Sets the duration of time before shields start
    recharging upon being hit. */
    void SetShieldRegenCooldown(float cd) { m_regenCD = cd; }
    
    /* Sets the amount of shields regenerated per second
    when shields are actually recharging. */
    void SetShieldRegenRate(float regenAmount) { m_regenRate = regenAmount; }

    void SetGodMode(bool god) { m_godMode = god; }
    bool GetGodMode() { return m_godMode; }

    void SetRespawnGodMode(float time)
    { m_respawnGodTime = time; m_respawnGod = true; }

  public:
    std::string m_rechargingSound;
    std::string m_rechargedSound;

    /* Color of shields when they are full */
    static LightColor FullShieldColor;
    static LightColor FullShieldFadeColor;

    /* Color of shields when they are just above empty */
    static LightColor LowShieldColor;
    static LightColor MedShieldColor;

    /* Color of shield when it is gone */
    LightColor m_dangerColor = LightColor(1.0f, 0.1f, 0.0f, 1.0f);

  private:
    void ShieldsRecharging(GameObject* obj);
    void ShieldsRecharged(GameObject* obj);

    void UpdateShieldLight(GameObject* obj, float dt);

    void TrueDeath();

    float m_maxShields = 150.0f;
    float m_shield = m_maxShields;
    float m_nextRegen = 0.0f; /* Time until we can regenerate */
    float m_regenCD = 1.0f; /* Regeneration Cool down */
    float m_regenRate = 30.0f; /* Regeneration per second */
    float m_fadeTime = 5.0f;
    bool  m_rechargingShields = false;
    float m_nextDangerSwitch = 0.0f;
    float m_shieldFlash = 0.0f;
    
    float m_maxHealth = 50.0f;
    float m_health = m_maxHealth;

    bool m_dead = false;
    int m_killer = -1;

    bool m_godMode = false;
    bool m_respawnGod = true;
    float m_respawnGodTime = 3.0f;

    int m_playerNum = -1;

    GameSpace* m_space = nullptr;
    Handle m_owner = Handle::null;
    Handle m_spineLight = Handle::null;
    Handle m_soundEmitter = Handle::null;

  };
}