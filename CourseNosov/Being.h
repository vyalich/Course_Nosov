#pragma once

#include "Dynamic.h"

class Being: public Dynamic
{
protected:

	int		hp_cur;
	int		hp_max;
	int		hp_regen;
	SDL_TimerID	HPTimerID;

	void Regen();

	static Uint32 HPTimerCallback(Uint32 interval, void* data);

	void AnimDead();

	static Uint32 AnimDeadTimerCallback(Uint32 interval, void* data);


public:
	Being() {}
	virtual ~Being() {}

	virtual void Process() {}

	virtual void Clear() {}
};

