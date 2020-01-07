#include "Being.h"

Uint32 Being::HPTimerCallback(Uint32 interval, void* data)
{
	static_cast<Being*>(data)->Regen();
	return interval;
}


void Being::Regen()
{
	hp_cur += hp_regen;
	if (hp_cur > hp_max)
		hp_cur = hp_max;
}

Uint32 Being::AnimDeadTimerCallback(Uint32 interval, void* data)
{
	static_cast<Being*>(data)->AnimDead();
	return interval;
}

void Being::AnimDead()
{
	frame_y = state * 4  * 64;

	frame_x += 64;
	if (frame_x > last_frame[state])
	{
		frame_x = last_frame[state];
		SDL_RemoveTimer(AnimTimerID);
		onscreen = false;
	}
}
