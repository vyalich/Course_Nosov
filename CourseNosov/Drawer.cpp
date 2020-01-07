#include "Drawer.h"

vector<TTF_Font*> Drawer::Fonts;

SDL_Texture* Drawer::Load(SDL_Renderer* Render, const char* file)
{
	SDL_Texture* tmp = IMG_LoadTexture(Render, file);

	if (!tmp)
	{
		SDL_Log(u8"Невозможно загрузить изображение, ошибка: %s", IMG_GetError());
	}

	return tmp;
}

void Drawer::Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int W, int H)
{
	SDL_Rect dest = { X, Y, W, H};

	SDL_RenderCopy(Render, texture, 0, &dest);
}

void Drawer::Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int X2, int Y2, int W, int H)
{
	SDL_Rect dest = { X, Y, W, H};

	SDL_Rect src = { X2, Y2, W, H };

	SDL_RenderCopy(Render, texture, &src, &dest);

}

void Drawer::Draw(SDL_Renderer* Render, SDL_Texture* texture, int X, int Y, int X2, int Y2, int W, int H, double angle, SDL_RendererFlip flip)
{
	SDL_Rect dest = { X, Y, W, H };

	SDL_Rect src = { X2, Y2, W, H };

	SDL_Point center = { X + W / 2, Y + H / 2 };

	SDL_RenderCopyEx(Render, texture, &src, &dest, angle*180/M_PI - 90, 0, flip);

}

void Drawer::DrawText(SDL_Renderer* Render, int FontID, const char* title, int X, int Y, int W, int H, SDL_Color* color, SDL_Color* bg_color)
{
	//Render text surface
	SDL_Surface* textSurface;
	string n_title;
		
	if (title == "")
		n_title = u8" ";
	else
		n_title = title;

	if (bg_color)		{
		textSurface = TTF_RenderUTF8_Shaded(Fonts[FontID], title, *color, *bg_color);
	}
	else
	{
		textSurface = TTF_RenderUTF8_Blended(Fonts[FontID], title, *color);
	}

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

		SDL_Rect dest = { X + W/2 - textSurface->w/2, Y + H/2 - textSurface->h/2, textSurface->w, textSurface->h };
		/*SDL_Texture* texTarget = SDL_CreateTexture(Render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textSurface->w, textSurface->h);

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
		textSurface = nullptr;*/

		SDL_RenderCopy(Render, texture, 0, &dest);
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(textSurface);
	}
}

int Drawer::LoadFonts()
{
	ifstream in;
	string buf;
	int n;
	char c;
	TTF_Font* tmp;

	in.open("fonts.dat", ios_base::binary);

	int i = 0;

	while (in.read(&c, sizeof(c)))
	{
		buf.clear();
		while (c)
		{
			buf += c;
			in.read(&c, sizeof(c));
		}
		in.read((char*)& n, sizeof(n));

		tmp = TTF_OpenFont(buf.c_str(), n);

		if (tmp == NULL)
		{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно открыть шрифт %d, ошибка: %s", i+1, TTF_GetError());
			return -1;
		}

		Fonts.push_back(tmp);

		i++;
	}
	return 0;
}