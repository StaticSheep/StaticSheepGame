#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"
#include "../controllers/player/CPlayerController.h"

namespace Framework
{
  //this enum is for convenience so I don't have to scope into the gamepad to get its enums for buttons
  enum Buttons{
    A,      //0
    B,      //1
    X,      //2
    Y,      //3
    UP,     //4
    DOWN,   //5
    LEFT,   //6
    RIGHT,  //7
    LT,  //8
    RT, //9
    LB,  //10
    RB, //11
    START,  //12
    SELECT,  //13
    TOTALBUTTONS //14
  };

	class Cheats : public GameComponent
	{
  public:
    //this is a cheat sequence, you dont have to fuck with this stuff
    struct CheatSequence{
      CheatSequence() {
        for (int i = 0; i < 6; ++i)
          activateSequence[i] = false;
      };
      CheatSequence(Buttons *);
      void CheatUpdate(Buttons button);
      void ClearSequence();
      Buttons buttonSequence[6];
      bool activateSequence[6];
      bool activated;
    };

    Cheats();
    ~Cheats();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void RecordButtonPress(Buttons button);
    Handle playerController;
    PlayerController *pc;
		//member variables
    bool enabled;     //are cheats turned on?
    //add new cheat sequences here!
    CheatSequence GM, GG, PM;

	};
}