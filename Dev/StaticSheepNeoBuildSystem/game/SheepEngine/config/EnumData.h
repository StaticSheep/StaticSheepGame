/*****************************************************************
Filename: EnumData.h
Project: 
Author(s): Zachary Nawar (Primary)

This is the enumeration manifest file where special enumerations
are processed by the engine. Essentially this is what happens to
every single enumeration declared here.

(Where *** is what is inside E_NAME() )
(Where $$$ is what is inside E_ENTRY() )

- Creates a C++ enumeration E*** 
  - Each Entry inside of the enumeration is e$$$
- Creates an instance of the Enum class named ENUM***

The Enum class is cool because it contains a hash map of $$$
to the corresponding C++ enumeration index

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

E_NAME(Generics)
  E_ENTRY(GameComponent)
  E_ENTRY(GameObject)
  E_ENTRY(Abstract)
E_END(Generics)

E_NAME(Weapons)
  E_ENTRY(Pistol)
  E_ENTRY(Automatic)
  E_ENTRY(Shotgun)
  E_ENTRY(WLaser)
  E_ENTRY(Missile)
  E_ENTRY(NoWeapon) //keep this at the end
E_END(Weapons)

E_NAME(PowerUps)
  E_ENTRY(Shield)
  E_ENTRY(Power)
  E_ENTRY(Coin)
  E_ENTRY(Explosive)
  E_ENTRY(MiniMe)
  E_ENTRY(NoPowerUp) //keep this at the end
E_END(PowerUps)

E_NAME(Component)
  E_ENTRY(Transform)
  E_ENTRY(div_Graphics)
  E_ENTRY(Sprite)
  E_ENTRY(AniSprite)
  E_ENTRY(SpineSprite)
  E_ENTRY(Camera)

  E_ENTRY(div_Lighting)
  E_ENTRY(PointLight)
  E_ENTRY(SpriteLight)
  E_ENTRY(SpineLight)
  E_ENTRY(BlockLights)
  E_ENTRY(Level1_Lighting)
  
  E_ENTRY(div_Particles)
  E_ENTRY(BasicParticleSystem)
  E_ENTRY(ParticleSystem)
  E_ENTRY(ParticleCircleEmitter)
  E_ENTRY(ParticleBoxEmitter)

  E_ENTRY(div_Physics)
  E_ENTRY(BoxCollider)
  E_ENTRY(CircleCollider)

  E_ENTRY(div_Audio)
  E_ENTRY(SoundEmitter)
  E_ENTRY(SoundPlayer)

  E_ENTRY(div_SlotMachines)
  E_ENTRY(SlotMachine)
  E_ENTRY(PersonalSlotController)

  E_ENTRY(div_Controllers)
  E_ENTRY(SlotController)
  E_ENTRY(PlayerController)
  E_ENTRY(ChipController)
  E_ENTRY(RoundController)
  E_ENTRY(LobbyController)
  E_ENTRY(JoinText)
  
  E_ENTRY(div_Effects)
  E_ENTRY(AimingArrow)
  E_ENTRY(DashEffect)
  E_ENTRY(BackgroundPan)
  E_ENTRY(WarningText)
  E_ENTRY(Explosion)
  E_ENTRY(ImpactEffect)
  E_ENTRY(RoundText)
  E_ENTRY(RoundResults)
  E_ENTRY(Pulser)

  E_ENTRY(div_Gameplay)
  E_ENTRY(PowerupPickup)
  E_ENTRY(Asteroid)
  E_ENTRY(GamePad)
  E_ENTRY(WeaponPickup)

  E_ENTRY(div_Weaponry)
  E_ENTRY(Bullet_Default)
  E_ENTRY(AOEDamage)
  
  E_ENTRY(div_LevelLogic)
  E_ENTRY(Level1_Logic)
  E_ENTRY(Grinder)
  E_ENTRY(Pinwheel)  
  E_ENTRY(Cheats)
  E_ENTRY(ElevatorPlat)
  E_ENTRY(GiantKillBox)
  E_ENTRY(Saucer)
  E_ENTRY(SaucerMissile)
  E_ENTRY(JuggernautEffect)
  E_ENTRY(Laser)
  E_ENTRY(CoinBall)

  E_ENTRY(div_Beta)
  E_ENTRY(Tester)

  

  E_ENTRY(LuaComponent)
  E_ENTRY(countComponents) //keep this at the end
E_END(Component)