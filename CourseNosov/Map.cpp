#include "Map.h"

Map Map::MapControl;
SDL_Texture* Map::Tileset;

void Map::Load() {
	Tileset_w = 6;

	bool* Map = new bool[MAP_H * MAP_W];
	//заполнение карты рандомными клетками
	for (int i = 0; i < MAP_H; i++)
		for (int j = 0; j < MAP_W; j++)
			Map[i * MAP_W + j] = rand() % 1000 < 460;

	for(int i = 0; i < LOOPS; i++)
		Map = MapCreate(Map);
	
	for (int i = 0; i < MAP_W; i++)
	{
		Map[i] = true;
		Map[(MAP_H - 1) * MAP_W + i] = true;
	}

	for (int i = 0; i < MAP_H; i++)
	{
		Map[MAP_W*i] = true;
		Map[MAP_W*i + MAP_W - 1] = true;
	}

	Tile tmp;
	for (int i = 0; i < MAP_H; i++) {
		for (int j = 0; j < MAP_W; j++) {
			if (Map[i * MAP_W + j]) {
				tmp.ID = 1;

				if (i > 0 && !Map[(i - 1) * MAP_W + j])
					tmp.ID = 4;
				if (i < MAP_H - 1 && !Map[(i + 1) * MAP_W + j])
					tmp.ID += 2;
				if (i < MAP_H - 2 && !Map[(i + 2) * MAP_W + j] && Map[(i + 1) * MAP_W + j])
					tmp.ID++;
				if (j > 0 && (!Map[i * MAP_W + j - 1] || Map[i * MAP_W + j - 1] && !Map[(i + 1) * MAP_W + j - 1] && Map[(i + 1) * MAP_W + j] && i < MAP_H - 1))
					tmp.ID += Tileset_w;
				if (j < MAP_W - 1 && (!Map[i * MAP_W + j + 1] || Map[i * MAP_W + j + 1] && !Map[(i + 1) * MAP_W + j + 1] && Map[(i + 1) * MAP_W + j] && i < MAP_H - 1))
					tmp.ID += 2 * Tileset_w;

				tmp.Type = 1;
				TileList.push_back(tmp);

			}
			else {
				tmp.ID = 0;
				tmp.Type = 0;
				TileList.push_back(tmp);

			}
		}
	}

	delete Map;

	bool done = false;

	int id;

	while (!done)
	{
		id = 140 + rand() % 13580;
		if (TileList[id].ID == 3 && TileList[id + 1].ID == 3 && TileList[id-MAP_W].ID == 2 && TileList[id - MAP_W + 1].ID == 2)
		{
			TileList[id].ID = 25;
			TileList[id].Type = TILE_EXIT;
			TileList[id+1].ID = 26;
			TileList[id+1].Type = TILE_EXIT;
			TileList[id - MAP_W].ID = 27;
			TileList[id - MAP_W].Type = TILE_EXIT;
			TileList[id - MAP_W + 1].ID = 28;
			TileList[id - MAP_W + 1].Type = TILE_EXIT;
			break;
		}
	}
	SDL_Log("%d %d %d %d %d", id, id % MAP_W, id / MAP_W, id % MAP_W * TILE_SIZE, id % MAP_W * TILE_SIZE);
}

bool Map::MapCreatorCheck(bool* element, int y, int x) {
	int count = 0;

	for (int i = y - 1; i < y + 2; i++) {
		for (int j = x - 1; j < x + 2; j++) {
			if (i == y && j == x)
				continue;
			if (i < 0 || i >= MAP_H - 1 || j < 0 || j >= MAP_W - 1)
				count++;
			else if (element[i * MAP_W + j])
				count++;
		}
	}
	if (element[y * MAP_W + x]) {
		if (count < DEADBOT || count > DEADTOP)
			return false;
		else
			return true;
	}
	else {
		if (count > BIRTH)
			return true;
	}
	return element[y * MAP_W + x];
}

//создание новой карты
bool* Map::MapCreate(bool* MapOld) {
	bool* MapNew = new bool[MAP_H * MAP_W];

	for (int i = 0; i < MAP_H; i++)
		for (int j = 0; j < MAP_W; j++)
			MapNew[i * MAP_W + j] = MapCreatorCheck(MapOld, i, j);
	delete MapOld;
	return MapNew;
}

int	Map::LoadStatic(SDL_Renderer* Render, const char* tileset_path)
{
	Tileset = Drawer::Load(Render, tileset_path);

	if (!Tileset)
		return -1;

	return 0;
}

void Map::Draw(SDL_Renderer* Render) {
	if (Tileset == NULL) return;

	int FirstID = (int)x / TILE_SIZE + (int)y / TILE_SIZE * MAP_W;
	
	int max_y = y + WIN_H;
	int max_x = x + WIN_W;
	int ID;
	//отрисовка карты
	for (int scr_y = FirstID / MAP_W * TILE_SIZE - y; scr_y <= max_y; scr_y += TILE_SIZE) {
		ID = FirstID;
		//SDL_Log("%d", ID);
		for (int scr_x = ID % MAP_W * TILE_SIZE - x; scr_x <= max_x; scr_x += TILE_SIZE) {
			if (ID >= 14000 || ID < 0)
				return;
			//выбираем нужный тайл из тайлсета
			int tile_x = (TileList[ID].ID % Tileset_w) * TILE_SIZE;
			int tile_y = (TileList[ID].ID / Tileset_w) * TILE_SIZE;
			
			//отрисовка тайла
			Drawer::Draw(Render, Tileset, scr_x, scr_y, tile_x, tile_y, TILE_SIZE, TILE_SIZE);

			ID++;
		}
		FirstID += MAP_W;
	}
	//Drawer::Draw(Render, Tileset, 0, 0, TILE_SIZE*6, TILE_SIZE*8);
}

void Map::Clear() {
	if (Tileset) {
		SDL_DestroyTexture(Tileset);
		TileList.clear();
	}
}

int Map::GetTileType(int ID) {
	return TileList[ID].Type;
}

void Map::MoveCam(int n_x, int n_y)
{
	if (n_x >= 0 && n_x + WIN_W < MAP_W_PX)
		x = n_x;
	if (n_y >= 0 && n_y + WIN_H < MAP_H_PX)
		y = n_y;
}