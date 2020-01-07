#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Drawer.h"

enum
{
	EXP = 0,
	HEALTH,
	MANA,
	ENEMY
};

struct Bar
{
	int x, y, w, h;
	Uint8 r, g, b;
	int max;
	int cur;
	std::string info;
	SDL_Color TextColor;
	SDL_Color* TextBGColor;
	SDL_Texture* Background;
	bool reversed;


	Bar() {}

	int  LoadBG(SDL_Renderer* Render, const char* path);
	void Draw(SDL_Renderer* Render);
};

class GUI {
	friend class Player;
	//friend class Enemy;
private:
	Bar       BarList[3];

public:
	static GUI GUIControl;

	GUI() {};

	int Load(SDL_Renderer* Render);
	void Draw(SDL_Renderer* Render);
	void Process();
	void Clear();
};