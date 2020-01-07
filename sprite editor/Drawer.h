#pragma once
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Drawer {
public:
	Drawer() {}

public:
	static SDL_Texture* Load(SDL_Renderer* Render, const char* File);

	static bool Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int W, int H);

	static bool Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int X2, int Y2, int W, int H);

	static void DrawText(SDL_Renderer* Render, TTF_Font* Font, const char* title, int X, int Y);
};

