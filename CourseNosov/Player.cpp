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

	hp_cur = 100;
	hp_regen = 3;
	hp_max = 100;

	mp_cur = 100;
	mp_regen = 5;
	mp_max = 100;

	exp_cur = 0;
	exp_max = 100;

	level = 1;

	MPTimerID = SDL_AddTimer(1000, MPTimerCallback, this);
	HPTimerID = SDL_AddTimer(1000, HPTimerCallback, this);
	AnimTimerID = SDL_AddTimer(100, AnimTimerCallback, this);

	return 0;
}

void Player::Process()
{
	GUI::GUIControl.BarList[GUI_HEALTH].max = hp_max;
	GUI::GUIControl.BarList[GUI_HEALTH].cur = hp_cur;
	GUI::GUIControl.BarList[GUI_MANA].max = mp_max;
	GUI::GUIControl.BarList[GUI_MANA].cur = mp_cur;
	GUI::GUIControl.BarList[GUI_EXP].max = exp_max;
	GUI::GUIControl.BarList[GUI_EXP].cur = exp_cur;
	Move();
	Map::MapControl.MoveCam(x - WIN_W / 2 + w / 2, y - WIN_H / 2 + h / 2);

	if (exp_cur >= exp_max)
	{
		exp_cur -= exp_max;
		exp_max *= 1.2;
		level++;
	}
	
}


void Player::Clear()
{

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