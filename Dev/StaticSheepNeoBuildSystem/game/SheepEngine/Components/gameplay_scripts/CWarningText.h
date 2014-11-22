#pragma once

#include "components/base/Component.h"
#include "types/handle/Handle.h"

namespace Framework
{
	class WarningText : public GameComponent
	{
	public:
    WarningText();
    ~WarningText();
		void LogicUpdate(float dt);
		void Initialize();
		void Remove();
    void DestroySelf();
    void RespawnBlink(float dt);

		//member variables
    Handle eTransfrom;
    Handle warnSprite;
    float timer;
    bool blink;
	};
}