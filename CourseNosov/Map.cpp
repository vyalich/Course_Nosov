#include "Map.h"

Map Map::MapControl;
SDL_Texture* Map::Tileset;

void Map::Load(const char* tilelist_path) {
	TileList.clear();

	std::ifstream in(tilelist_path, std::ios_base::binary);


	Tile tmp;
	for (int i = 0; i < MAP_H; i++) {
		for (int j = 0; j < MAP_W; j++) {
			in.read((char*)& tmp.ID, sizeof(tmp.ID));
			in.read((char*)& tmp.Type, sizeof(tmp.Type));
			TileList.emplace_back(tmp);
		}
	}

	in.close();

	Tileset_w = 6;

	x = y = 0;
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
	if(ID < 14000 && ID >= 0)
		return TileList[ID].Type;
	return TILE_BLOCK;
}

void Map::MoveCam(int n_x, int n_y)
{
	if (n_x >= 0 && n_x + WIN_W < MAP_W_PX)
		x = n_x;
	if (n_y >= 0 && n_y + WIN_H < MAP_H_PX)
		y = n_y;
}