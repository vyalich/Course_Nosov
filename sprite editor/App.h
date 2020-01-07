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
	//�������������
	int Init();

	//������������ ������
	void Clear();

	//������� ����
	int Execute();

	//��������� �������
	void Handle();

	//������� ������
	void Process();

	//���������
	void Draw();

	//��������� ����������� 9�4
	void AddIdleWalk(const char* path, int cnt);

	//��������� ����������� 8�4
	void AddAttack(const char* path, int cnt);

	//��������� ����������� 1�4
	void AddDie(const char* path, int cnt);

	void Save(const char* path, SDL_Texture* img);

	const char* InputText();

};

