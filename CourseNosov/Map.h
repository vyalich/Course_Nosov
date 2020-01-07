#pragma once

#include <SDL.h>
#include <vector>
#include <fstream>
#include "Define.h"
#include "Drawer.h"

class Tile {
public:
	int     ID;
	int     Type;

public:
	Tile() {}
};

class Map {
private:
	static SDL_Texture*		Tileset;      //tileset
	std::vector<Tile>	TileList;     //

	int			Tileset_w;
	double		x, y;

public:
	
	Map() {}

	void Load(const char* tilelist_path);

	static int	LoadStatic(SDL_Renderer* Render, const char* tileset_path);

	void Draw(SDL_Renderer* Render);

	void Clear();

	void MoveCam(int dx, int dy);

	int  GetTileType(int ID);

	static Map MapControl;

	double GetX() { return x; }

	double GetY() { return y; }
};

