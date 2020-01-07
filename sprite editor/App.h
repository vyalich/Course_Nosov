#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string>
#include "Drawer.h"

using namespace std;

class App
{
private:
	SDL_Window*		Window;
	SDL_Renderer*	Render;
	TTF_Font*		Font;
	SDL_Texture*	result;
	SDL_Texture*	bar;
	string			way;
	//SDL_Texture** data[100];

	SDL_Event	e;

	bool running;

public:
	//инициализация
	int Init();

	//освобождение памяти
	void Clear();

	//главный цикл
	int Execute();

	//обработка событий
	void Handle();

	//игровая логика
	void Process();

	//отрисовка
	void Draw();

	//загрузить изображение 9х4
	void AddIdleWalk(const char* path, int cnt);

	//загрузить изображение 8х4
	void AddAttack(const char* path, int cnt);

	//загрузить изображение 1х4
	void AddDie(const char* path, int cnt);

	void Save(const char* path, SDL_Texture* img);

	const char* InputText();

};

