#include "App.h"

int App::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать SDL2. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать SDL 2, ошибка: %s", SDL_GetError());
		return -10;
	}

	atexit(SDL_Quit);

	if (TTF_Init() < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать TTF. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать TTF, ошибка: %s", TTF_GetError());
		return -20;
	}

	atexit(TTF_Quit);

	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать TTF. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать TTF, ошибка: %s", TTF_GetError());
		return -20;
	}

	atexit(IMG_Quit);

	Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
	if (Window == 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно создать окно. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно создать окно, ошибка: %s", SDL_GetError());
		return -30;
	}

	//Create renderer for window
	Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (Render == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно создать рендерер. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно создать рендерер, ошибка: %s", SDL_GetError());
		return -40;
	}

	Font = TTF_OpenFont("a_Alterna.ttf", 20);
	if (Font == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно загрузить шрифт. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно загрузить шрифт, ошибка: %s", TTF_GetError());
		return -50;
	}

	//Initialize renderer color
	

	result = SDL_CreateTexture(Render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 512, 512 + 256 + 64);

	SDL_SetRenderDrawColor(Render, 255, 0, 255, 0);
	SDL_SetRenderTarget(Render, result);
	SDL_RenderFillRect(Render, 0);

	SDL_SetRenderTarget(Render, 0);

	SDL_Texture* tmp_tex = SDL_CreateTexture(Render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 40);
	SDL_SetRenderTarget(Render, tmp_tex);
	SDL_RenderFillRect(Render, 0);

	int width, height;
	SDL_QueryTexture(tmp_tex, NULL, NULL, &width, &height);
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_RenderReadPixels(Render, NULL, surface->format->format, surface->pixels, surface->pitch);
	SDL_SetColorKey(surface, SDL_RLEACCEL, 0xff00ff);
	bar = SDL_CreateTextureFromSurface(Render, surface);

	SDL_DestroyTexture(tmp_tex);
	SDL_FreeSurface(surface);

	/*SDL_SetRenderTarget(Render, bar);
	SDL_SetRenderDrawColor(Render, 0, 0, 255, 128);
	SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(Render, 0);

	SDL_SetRenderTarget(Render, 0);*/
	//SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);

	running = true;

	return 0;
}

int App::Execute()
{
	if (Init() < 0)
		return -100;

	while (running)
	{
		SDL_WaitEvent(&e);
		Handle();
		Draw();
	}

	return 0;
}

void App::Handle()
{
	switch (e.key.keysym.sym)
	{
	case SDLK_1:
	{
		string file = InputText();
		string path = "./png/walkcycle/" + file;
		AddIdleWalk(path.c_str(), 9);
		path = "./png/slash/" + file;
		AddAttack(path.c_str(), 6);
		path = "./png/hurt/" + file;
		AddDie(path.c_str(), 6);
		break;
	}
	case SDLK_2:
	{
		string file = InputText();
		string path = "./png/walkcycle/" + file;
		AddIdleWalk(path.c_str(), 9);
		path = "./png/thrust/" + file;
		AddAttack(path.c_str(), 8);
		path = "./png/hurt/" + file;
		AddDie(path.c_str(), 6);
		break;
	}
	case SDLK_3:
	{
		string file = InputText();
		string path = "./png/walkcycle/" + file;
		AddIdleWalk(path.c_str(), 9);
		path = "./png/spellcast/" + file;
		AddAttack(path.c_str(), 8);
		path = "./png/hurt/" + file;
		AddDie(path.c_str(), 6);
		break;
	}
	case SDLK_q:
		Save(InputText(), result);
		break;
	case SDLK_w:
		Save(InputText(), bar);
		break;
	case SDLK_ESCAPE:
		running = false;
	}
}

void App::Draw()
{
	SDL_SetRenderDrawColor(Render, 255, 0, 0, 255);
	SDL_RenderClear(Render);

	SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_BLEND);
	Drawer::Draw(Render, result, 0, 0, 512, 512 + 256 + 64);
	Drawer::Draw(Render, bar, 200, 20, 300, 40);

	SDL_RenderPresent(Render);
}

const char* App::InputText()
{
	
	bool done = false;
	SDL_Rect rect = { 600, 20, 500, 50 };
	SDL_Event event;

	SDL_StartTextInput();
	way.clear();
	SDL_Log("start text input");

	SDL_WaitEvent(&event);

	while (!done)
	{
		
		if(SDL_WaitEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				//SDL_Log("smth pressed");
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					done = true;
					//SDL_Log("Oh my, enter pressed");
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE && way.length() > 0)
				{
					//lop off character
					way.pop_back();
				}
				break;
			case SDL_TEXTINPUT:
				//SDL_Log("smth inputed");
				way += event.text.text;
				
				SDL_Log("%s", way.c_str());
				break;
			}			
		}

		SDL_SetRenderDrawColor(Render, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderFillRect(Render, &rect);
	
		Drawer::DrawText(Render, Font, way.c_str(), 600, 35);

		SDL_RenderPresent(Render);
	}
	SDL_StopTextInput();
	
	return way.c_str();
}

void App::AddIdleWalk(const char* path, int cnt)
{
	SDL_Texture* tmp = Drawer::Load(Render, path);
	SDL_Rect IDLE = { 0, 0, 64, 256 };
	SDL_Rect srcWalk = { 64, 0, 64*(cnt-1), 256 };
	SDL_Rect dstWalk = { 0, 256, 64*(cnt-1), 256 };
	SDL_SetRenderTarget(Render, result);

	SDL_RenderCopy(Render, tmp, &IDLE, &IDLE);
	SDL_RenderCopy(Render, tmp, &srcWalk, &dstWalk);

	SDL_SetRenderTarget(Render, 0);
	SDL_DestroyTexture(tmp);
	
}

void App::AddAttack(const char* path, int cnt)
{
	SDL_Texture* tmp = Drawer::Load(Render, path);
	SDL_Rect src = { 128, 0, 128, 256 };
	SDL_Rect dst = { 0, 512, 128, 256 };
	SDL_SetRenderTarget(Render, result);

	SDL_RenderCopy(Render, tmp, &src, &dst);

	SDL_Rect src2 = { 320, 0, 64, 256 };
	SDL_Rect dst2 = { 128, 512, 64, 256 };

	SDL_RenderCopy(Render, tmp, &src2, &dst2);

	SDL_SetRenderTarget(Render, 0);
	SDL_DestroyTexture(tmp);
}

void App::AddDie(const char* path, int cnt)
{
	SDL_Texture* tmp = Drawer::Load(Render, path);
	SDL_Rect src = { 0, 0, 64*cnt, 64 };
	SDL_Rect dst = { 0, 512 + 256, 64*cnt, 64 };
	SDL_SetRenderTarget(Render, result);

	SDL_RenderCopy(Render, tmp, &src, &dst);

	SDL_SetRenderTarget(Render, 0);
	SDL_DestroyTexture(tmp);
}

void App::Save(const char* path, SDL_Texture* img)
{
	SDL_Texture* target = SDL_GetRenderTarget(Render);
	SDL_SetRenderTarget(Render, img);
	SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_NONE);
	int width, height;
	SDL_QueryTexture(img, NULL, NULL, &width, &height);
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	//SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
	SDL_RenderReadPixels(Render, NULL, surface->format->format, surface->pixels, surface->pitch);
	SDL_SetColorKey(surface, SDL_RLEACCEL, 0xff00ff);
	IMG_SavePNG(surface, path);
	SDL_FreeSurface(surface);
	SDL_SetRenderTarget(Render, target);
}

int main(int argc, char* argv[])
{
	App app;
	return app.Execute();
}