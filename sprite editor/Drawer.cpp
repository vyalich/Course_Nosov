#include "Drawer.h"

SDL_Texture* Drawer::Load(SDL_Renderer* Render, const char* file)
{
	SDL_Texture* tmp = IMG_LoadTexture(Render, file);

	if (!tmp)
	{
		SDL_Log(u8"Невозможно загрузить изображение, ошибка: %s", IMG_GetError());
	}

	return tmp;
}

bool Drawer::Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int W, int H)
{
	if (!texture) {
		return false;
	}

	SDL_Rect dest = { X, Y, W, H };

	SDL_RenderCopy(Render, texture, 0, &dest);

	return true;
}

bool Drawer::Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int X2, int Y2, int W, int H)
{
	if (!texture) {
		return false;
	}

	SDL_Rect dest = { X, Y, W, H };

	SDL_Rect src = { X2, Y2, W, H };

	SDL_RenderCopy(Render, texture, &src, &dest);

	return true;
}

void Drawer::DrawText(SDL_Renderer* Render, TTF_Font* Font, const char* title, int X, int Y)
{
	//Render text surface
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface* textSurface;

	if(title != "")
		textSurface = TTF_RenderUTF8_Blended(Font, title, color);
	else
		textSurface = TTF_RenderUTF8_Blended(Font, u8" ", color);

	if (textSurface == 0)
	{
		SDL_Log("Unable to render text surface! SDL_ttf Error: %s", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		SDL_Texture* texture = SDL_CreateTextureFromSurface(Render, textSurface);

		if (texture == 0)
		{
			SDL_Log("Unable to create texture from rendered text! SDL Error: %s", SDL_GetError());
			return;
		}

		SDL_Rect dest = { X, Y, textSurface->w, textSurface->h };
		SDL_Texture* texTarget = SDL_CreateTexture(Render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textSurface->w, textSurface->h);

		//Now render to the texture
		SDL_SetRenderTarget(Render, texTarget);
		SDL_RenderClear(Render);
		SDL_RenderCopy(Render, texture, NULL, NULL);
		//Detach the texture
		SDL_SetRenderTarget(Render, NULL);

		SDL_RenderCopy(Render, texTarget, 0, &dest);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(texture);
		SDL_DestroyTexture(texTarget);
		texture = nullptr;
		textSurface = nullptr;
	}
}