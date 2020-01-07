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

	//�������������
	int Init();

	//������������� ������
	int LevelInit();
	
	//������������ ������
	void Clear();
	
	//������� ����
	int MainCycle();

	//��������� �������
	void Handle(SDL_Event* e);

	//������� ������
	void Process();

	//���������
	void Draw();

	static Uint32 MainCycleTimerCallback(Uint32 interval, void* data);

private:
	SDL_Window*		Window;
	SDL_Renderer*	Render;
	SDL_TimerID		MainCycleTimerID;
	int				_running;

	int				enemy_cnt;

	bool			_lmb;	//����� ������ ����
	bool			_rmb;	//������ ������ ����
};

