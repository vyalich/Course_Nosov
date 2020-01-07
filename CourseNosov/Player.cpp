#include "Player.h"
#include "GUI.h"

Player Player::player;

int Player::Load(const char* path, SDL_Renderer* Render)
{
	dx = dy = 0;
	x = 500;
	y = 500;
	w = h = 64;
	speed = 4;

	onscreen = true;

	col_l = 16, col_t = 42, col_r = 16, col_b = 2;

	sprite_sheet = Drawer::Load(Render, path);

	if (!sprite_sheet)
	{
		SDL_Log(u8"не удалось загрузить спарйты игрока");
		return -1;
	}

	frame_x = frame_y = 0;
	state = BEING_IDLE;
	direction = BOTTOM;

	last_frame[BEING_IDLE] = 0;
	last_frame[BEING_CHASE] = 64 * 7;
	last_frame[BEING_ATTACK] = 64 * 2;
	last_frame[BEING_DEAD] = 64 * 5;

	hp_cur = 80;
	hp_regen = 1;
	hp_max = 80;

	mp_cur = 120;
	mp_regen = 3;
	mp_max = 120;

	exp_cur = 0;
	exp_max = 100;

	level = 1;

	intellect = 2;

	MPTimerID = SDL_AddTimer(1000, MPTimerCallback, this);
	HPTimerID = SDL_AddTimer(1000, HPTimerCallback, this);
	AnimTimerID = SDL_AddTimer(100, AnimTimerCallback, this);

	return 0;
}

void Player::LevelInit()
{
	int id;

	while (true)
	{
		id = 140 + rand() % 13580;
		if (Map::MapControl.GetTileType(id) == TILE_FLOOR && Map::MapControl.GetTileType(id + 1) == TILE_FLOOR && 
			Map::MapControl.GetTileType(id + MAP_W) == TILE_FLOOR && Map::MapControl.GetTileType(id + MAP_W + 1) == TILE_FLOOR)
		{
			x = id % MAP_W * TILE_SIZE;
			y = id / MAP_W * TILE_SIZE;
			break;
		}
	}
}

bool Player::Process()
{
	if (!onscreen)
		return false;

	if (hp_cur <= 0 && state != BEING_DEAD) {
		Clear();
		GUI::GUIControl.BarList[GUI_HEALTH].cur = 0;
		AnimTimerID = SDL_AddTimer(200, AnimDeadTimerCallback, this);
		state = BEING_DEAD;
	}

	if (state == BEING_DEAD)
		return true;

	bool flag;
	GUI::GUIControl.BarList[GUI_HEALTH].max = hp_max;
	GUI::GUIControl.BarList[GUI_HEALTH].cur = hp_cur;
	GUI::GUIControl.BarList[GUI_MANA].max = mp_max;
	GUI::GUIControl.BarList[GUI_MANA].cur = mp_cur;
	GUI::GUIControl.BarList[GUI_EXP].max = exp_max;
	GUI::GUIControl.BarList[GUI_EXP].cur = exp_cur;

	flag = Move();

	Map::MapControl.MoveCam(x - WIN_W / 2 + w / 2, y - WIN_H / 2 + h / 2);

	if (exp_cur >= exp_max)
	{
		exp_cur -= exp_max;
		exp_max *= 1.2;
		hp_max *= 1.05;
		hp_cur = hp_max;
		mp_max *= 1.1;
		mp_cur = mp_max;
		level++;
		if (level % 3 == 0)
			intellect += level;
	}



	return flag;
}

Uint32 Player::MPTimerCallback(Uint32 interval, void* data) 
{ 
	static_cast<Player*>(data)->MPRegen(); 
	return interval; 
}

void Player::MPRegen()
{
	mp_cur += mp_regen;
	if (mp_cur > mp_max)
		mp_cur = mp_max;
}

bool Player::Move()
{
	x += dx;

	PosValidX();

	y += dy;

	if (!PosValidY() && dy < 0)
	{
		int id = (int)(x + col_l) / TILE_SIZE + (int)(y + col_t) / TILE_SIZE * MAP_W;
		if (Map::MapControl.GetTileType(id-MAP_W) == TILE_EXIT || Map::MapControl.GetTileType(id - MAP_W + 1) == TILE_EXIT)
		{
			return false;
		}
	}
	return true;
}