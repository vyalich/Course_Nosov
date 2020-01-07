#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Drawer {
private:

	static vector<TTF_Font*> Fonts;

public:
	Drawer() {}


	static int LoadFonts();

	static SDL_Texture* Load(SDL_Renderer* Render, const char* File);

	static void Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int W, int H);

	static void Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int X2, int Y2, int W, int H);

	static void Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int X2, int Y2, int W, int H, double angle, SDL_RendererFlip flip);

	static void DrawText(SDL_Renderer* Render, int Font, const char* title, int X, int Y, int W, int H, SDL_Color* color, SDL_Color* bg_color);
};

