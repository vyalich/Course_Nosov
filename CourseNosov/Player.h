#pragma once

#include "Being.h"

class Enemy;
class Spell;

class Player: public Being
{
	friend class Enemy;
	friend class Spell;

protected:
	int mp_cur;
	int mp_max;
	int mp_regen;

	int exp_cur;
	int exp_max;

	int intellect;

	SDL_TimerID MPTimerID;

	int level;

	SDL_Texture* sprite_sheet;

	SDL_Texture* GetSpriteSheet() { return Player::sprite_sheet; }

	void MPRegen();

	static Uint32 MPTimerCallback(Uint32 interval, void* data);

public:
	Player() {}
	~Player() {}

	static Player player;

	bool Alive() { return onscreen; }

	bool Move();

	int Load(const char* path, SDL_Renderer* Render);

	void LevelInit();

	bool Process();

	void Clear() { Being::Clear(); SDL_RemoveTimer(MPTimerID); }

	double GetX() { return x; }

	double GetY() { return y; }
};

