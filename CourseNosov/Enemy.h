#pragma once

#include <SDL.h>
#include "Drawer.h"
#include "Being.h"
#include "Player.h"

class Spell;

class Enemy: public Being
{
	friend class Spell;

protected:
	bool   _mouseover;

	static double Dist;
	static Uint16 AggroRNG;
	static Uint16 FleeRNG;
	static Uint16 AttackRNG;
	Uint16 AttackPWR;
	Uint16 AttackSPD;
	Uint32 LastAttack;

	double last_x;
	double last_y;

	static SDL_Texture* sprite_sheet;	

	SDL_Texture* GetSpriteSheet() { return Enemy::sprite_sheet; };


public:

	static std::vector<Enemy*> EnemyList;



	void Attack();

	void SetSpeed(double DestX, double DestY);

	void LineOfSight(double PlayerX, double PlayerY, SDL_Renderer* Render);

	bool CheckDist(double t_x, double t_y);

	void Load();

	static int LoadStatic(SDL_Renderer* Render);

	void Process(double PlayerX, double PlayerY, double CamX, double CamY, SDL_Renderer* Render);
};

