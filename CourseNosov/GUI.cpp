#include "GUI.h"

GUI GUI::GUIControl;

int Bar::LoadBG(SDL_Renderer* Render, const char* path)
{
	Background = Drawer::Load(Render, path);
	if (!Background)
		return -1;
	return 0;
}

void Bar::Draw(SDL_Renderer* Render)
{

	SDL_Rect dest;
	if (reversed)
	{
		dest.x = x + w - (w - 6) * cur / max  - 3, dest.y = y + 3, dest.w = (w - 6) * cur / max, dest.h = h - 6;
	}
	else
	{
		dest.x = x + 3, dest.y = y + 3, dest.w = (w - 6) * cur / max, dest.h = h - 6;
	}

	SDL_SetRenderDrawColor(Render, r, g, b, 255);
	SDL_RenderFillRect(Render, &dest);

	SDL_Rect dest2 = { x, y, w, h };

	SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(Render, Background, 0, &dest2);
	SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_NONE);

	SDL_SetRenderDrawColor(Render, 255, 255, 255, 1);
	info = to_string(cur) + "/" + to_string(max);
	Drawer::DrawText(Render, FONT_ALTERNA_16, info.c_str(), x, y, w, h, &TextColor, TextBGColor);
	SDL_SetRenderDrawBlendMode(Render, SDL_BLENDMODE_NONE);
}

int GUI::Load(SDL_Renderer* Render)
{
	ifstream in("gui.dat");
	string buf;
	int n;
	Uint8 sh;

	if (!in)
	{
		SDL_Log("Невозможно открыть файл GUI");
		return -1;
	}

	for (int i = 0; i < 3; i++)
	{
		in >> n;   BarList[i].x = n;

		in >> n;   BarList[i].y = n;
		in >> n;   BarList[i].w = n;
		in >> n;   BarList[i].h = n;
		in >> n;   BarList[i].r = n;
		in >> n;   BarList[i].g = n;
		in >> n;   BarList[i].b = n;
		in >> n;   BarList[i].TextColor.r = n;
		in >> n;   BarList[i].TextColor.g = n;
		in >> n;   BarList[i].TextColor.b = n;
		in >> n;   BarList[i].reversed = n;
		BarList[i].TextBGColor = 0;

		in.get();

		getline(in, buf);  
		if (BarList[i].LoadBG(Render, buf.c_str()) < 0)
			return -2;
	}

	BarList[GUI_EXP].TextBGColor = new SDL_Color({ 0, 0, 0, 255 });

	return 0;
}
void GUI::Draw(SDL_Renderer* Render)
{
	for (int i = 0; i < 3; i++)
		BarList[i].Draw(Render);
}

void GUI::Process() {}

void GUI::Clear()
{
	delete BarList[GUI_EXP].TextBGColor;
}