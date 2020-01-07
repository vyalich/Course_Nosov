#include "Enemy.h"
#include "Player.h"

std::vector<Enemy*> Enemy::EnemyList;
SDL_Texture* Enemy::sprite_sheet;

double Enemy::Dist;
Uint16 Enemy::AggroRNG;
Uint16 Enemy::FleeRNG;
Uint16 Enemy::AttackRNG;

void Enemy::Load() {
	//on_screen = false;
	state = BEING_IDLE;
	w = 64;
	h = 64;
	AttackSPD = 1000;
	AttackPWR = 1;
	LastAttack = 0;

	hp_cur = 100;
	hp_max = 100;
	hp_regen = 1;

	direction = rand() % 4;

	dx = dy = 0;
	last_x = x = (int)TILE_SIZE + rand() % ((int)MAP_W - 3) * (int)TILE_SIZE;
	last_y = y = (int)TILE_SIZE + rand() % ((int)MAP_H - 3) * (int)TILE_SIZE;
	w = h = 64;
	speed = 2.5;

	col_l = 16, col_t = 42, col_r = 16, col_b = 2;

	frame_x = frame_y = 0;
	state = BEING_IDLE;
	direction = BOTTOM;

	last_frame[BEING_IDLE] = 0;
	last_frame[BEING_CHASE] = 64 * 7;
	last_frame[BEING_ATTACK] = 64 * 6;
	last_frame[BEING_DEAD] = 64 * 5;

	hp_cur = 100;
	hp_regen = 3;
	hp_max = 100;

	HPTimerID = SDL_AddTimer(1000, HPTimerCallback, this);
	AnimTimerID = SDL_AddTimer(100, AnimTimerCallback, this);
}



int Enemy::LoadStatic(SDL_Renderer* Render)
{
	Dist = 1000;
	AggroRNG = 9 * TILE_SIZE;
	FleeRNG = 12 * TILE_SIZE;
	AttackRNG = TILE_SIZE + 16;

	sprite_sheet = Drawer::Load(Render, "enemy.png");

	if (!sprite_sheet)
	{
		return -1;
	}
	return 0;
}

void Enemy::Process(double PlayerX, double PlayerY, double CamX, double CamY, SDL_Renderer* Render) {
	if (hp_cur > 0)
	{
		if (x >= CamX - w && x <= CamX + WIN_W && y >= CamY - h && y <= CamY + WIN_H) {
			if (!onscreen)
			{
				Entity::OnScreen.push_back(this);
				onscreen = true;
			}
		}
		else {
			if (onscreen)
				Entity::OnScreen.remove(this);
			onscreen = false;
		}

		if (!onscreen)
			return;
	
		/*int MouseX, MouseY, ScrX = x - CamX, ScrY = x - CamY;
		SDL_GetMouseState(&MouseX, &MouseY);
		if (MouseX >= ScrX + 16 && MouseX <= ScrX + w - 16 &&
			MouseY >= ScrY + 14 && MouseY <= ScrY + h - 2)
			_mouseover = true;
		else
			_mouseover = false;*/

		Dist = sqrt((PlayerX - x) * (PlayerX - x) + (PlayerY - y) * (PlayerY - y));

		if (Dist <= AggroRNG && (state == BEING_IDLE || state == BEING_FLEE))
			LineOfSight(PlayerX, PlayerY, Render);

		if (state == BEING_ATTACK && frame_x != 0) {
			angle_cos = (PlayerX - x) / Dist;
			if (y - PlayerY > 0)
				dy = -0.000000001;
			else
				dy = 0;
			//Animate();
			return;
		}

		if (state != BEING_IDLE) {
			if (state != BEING_ATTACK && Dist <= AttackRNG) {
				state = BEING_ATTACK;
				dx = 0;
				dy = 0;
				frame_x = 0;
				SDL_RemoveTimer(AnimTimerID);
				AnimTimerID = SDL_AddTimer(AttackSPD / 8, AnimTimerCallback, this);
				//State = 4;
			}
			else if (state != BEING_FLEE && Dist >= FleeRNG) {
				frame_x = 0;
				SDL_RemoveTimer(AnimTimerID);
				AnimTimerID = SDL_AddTimer(100, AnimTimerCallback, this);
				state = BEING_FLEE;

			}
			else if (state == BEING_ATTACK && Dist > AttackRNG) {
				frame_x = 0;
				SDL_RemoveTimer(AnimTimerID);
				AnimTimerID = SDL_AddTimer(100, AnimTimerCallback, this);
				state = BEING_CHASE;
				//State = 0;
			}
		}
	}
	else if(state != BEING_DEAD)
	{
		frame_x = 0;
		SDL_RemoveTimer(AnimTimerID);
		SDL_RemoveTimer(HPTimerID);
		AnimTimerID = SDL_AddTimer(200, AnimDeadTimerCallback, this);
		state = BEING_DEAD;
		Player::player.exp_cur += 10;
	}

	switch (state) {
	case BEING_CHASE:
		dx = speed / Dist * (PlayerX - x);
		dy = speed / Dist * (PlayerY - y);
		angle_cos = (PlayerX - x) / Dist;
		Move();
		break;
	case BEING_FLEE:
		SetSpeed(last_x, last_y);
		Move();
		if (abs(x - last_x) < TILE_SIZE && abs(y - last_y) < TILE_SIZE) {
			state = BEING_IDLE;
			Stop();
		}
		break;
	case BEING_IDLE:
		Stop();
		break;
	case BEING_ATTACK:
		Attack();
		break;
	case BEING_DEAD:
		
		break;
	}
}

void Enemy::LineOfSight(double PlayerX, double PlayerY, SDL_Renderer* Render) {
	double shift_x = TILE_SIZE / Dist * (PlayerX - x);
	double shift_y = TILE_SIZE / Dist * (PlayerY - y);
	for (double X = x + w / 2, Y = y + h / 2; abs(X - (PlayerX + w / 2)) > abs(shift_x)
		|| abs(Y - (PlayerY + h / 2)) > abs(shift_y); X += shift_x, Y += shift_y) 
	{

		
		SDL_SetRenderDrawColor(Render, 255, 0, 0, 255);
		SDL_Rect rect = { X - Map::MapControl.GetX() - 2, Y - Map::MapControl.GetY() - 2, 5, 5 };
		SDL_RenderFillRect(Render, &rect);
		
		int ID = (int)X / TILE_SIZE + (int)Y / TILE_SIZE * MAP_W;
		if (Map::MapControl.GetTileType(ID) == TILE_BLOCK) 
		{
			return;
		}
	}
	//state = BEING_CHASE;
	last_x = x;
	last_y = y;
}

bool Enemy::CheckDist(double t_x, double t_y)
{
	if (hp_cur <= 0)
		return false;

	double c_x = x + w / 2;
	double c_y = y + h / 2;
	
	return (abs(t_x - c_x) < 15 && abs(t_y - c_y) < 15);
}

void Enemy::SetSpeed(double DestX, double DestY) {
	Dist = sqrt((DestX - x) * (DestX - x) + (DestY - y) * (DestY - y));
	dx = speed / Dist * (DestX - x);
	dy = speed / Dist * (DestY - y);
	angle_cos = (DestX - x) / Dist;
}

void Enemy::Attack()
{
	if (SDL_GetTicks() - LastAttack < AttackSPD)
		return;
	LastAttack = SDL_GetTicks();
	Player::player.hp_cur -= AttackPWR;
	if (Player::player.hp_cur < 0)
	{
		Player::player.hp_cur = 0;
		//Player::player.state = BEING_DEAD;
	}
}
