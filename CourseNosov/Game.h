#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <ctime>
#include "Define.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "GUI.h"
#include "Spell.h"

class Game
{
public:
	

	Game() {}
	~Game() {}

	//инициализация
	int Init();

	//инициализация уровня
	int LevelInit();
	
	//освобождение памяти
	void Clear();
	
	//главный цикл
	int MainCycle();

	//обработка событий
	void Handle(SDL_Event* e);

	//игровая логика
	void Process();

	//отрисовка
	void Draw();

	static Uint32 MainCycleTimerCallback(Uint32 interval, void* data);

private:
	SDL_Window*		Window;
	SDL_Renderer*	Render;
	SDL_TimerID		MainCycleTimerID;
	int				_running;

	int				enemy_cnt;

	bool			_lmb;	//левая кнопка мыши
	bool			_rmb;	//правая кнопка мыши
};

