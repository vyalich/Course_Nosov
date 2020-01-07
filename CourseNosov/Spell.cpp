#include "Spell.h"

std::list<Spell*>   Spell::SpellCasted;

int                 Spell::Cost;
Uint16              Spell::Range;
Uint16              Spell::Power;
Uint16              Spell::CoolDown;
Uint32              Spell::LastUsed;
SDL_Texture*		Spell::sprite_sheet;


void Spell::Load() {
	//_casted = false;

	exist = true;
	col_t = 27;
	col_l = col_r = col_b = 0;
	speed = 5;
	x = Player::player.GetX() + 23;
	y = Player::player.GetY() + 32;
	w = 15;
	h = 15;
	LastUsed = 0;
	frame_x = 0;
	frame_y = 0;
	AnimTimerID = SDL_AddTimer(1000 / 60, AnimTimerCallback, this);
	state = SPELL_CASTED;

	last_frame[SPELL_CASTED] = w * 9;
	last_frame[SPELL_DESTROY] = w * 6;

}

int Spell::LoadStatic(SDL_Renderer* Render)
{
	Cost = 8;
	Range = 10 * TILE_SIZE;
	Power = 30;
	CoolDown = 500;
	LastUsed = 0;

	sprite_sheet = Drawer::Load(Render, "fireball.png");

	if (!sprite_sheet)
	{
		SDL_Log(u8"не удалось загрузить спарйты игрока");
		return -1;
	}

	return 0;
}

void Spell::AddCasted(double t_x, double t_y) {
	Spell* temp;

	if (LastUsed + CoolDown < SDL_GetTicks() && Player::player.mp_cur >= Cost) {
		temp = new Spell;
		
		temp->Load();

		Player::player.mp_cur -= Cost;

		LastUsed = SDL_GetTicks();

		temp->SetSpeed(t_x + Map::MapControl.GetX() - 7.5, t_y + Map::MapControl.GetY() - 7.5);

		SpellCasted.push_back(temp);
	}
}

void Spell::Process()
{
	Move();

	//bool collides = false;

	for (int i = 0; i < Enemy::EnemyList.size(); i++)
	{
		if (Enemy::EnemyList[i]->CheckDist(x + w / 2, y + h / 2) && state == SPELL_CASTED)
		{
			Enemy::EnemyList[i]->hp_cur -= Power;
			Enemy::EnemyList[i]->state = BEING_CHASE;
			Kill();
			return;
		}
	}

	//if (collides) Kill();
}

void Spell::Kill()
{
	state = SPELL_DESTROY;
	frame_x = 0;
	SDL_RemoveTimer(AnimTimerID);
	AnimTimerID = SDL_AddTimer(100, AnimTimerCallback, this);
	dx = dy = 0;
}

void Spell::Draw(SDL_Renderer* Render, int CamX, int CamY) 
{


		if(dy < 0)
			Drawer::Draw(Render, GetSpriteSheet(), x - CamX, y - CamY, frame_x, frame_y, w, h, M_PI - SDL_acos(angle_cos), SDL_FLIP_VERTICAL);
		else
			Drawer::Draw(Render, GetSpriteSheet(), x - CamX, y - CamY, frame_x, frame_y, w, h, SDL_acos(angle_cos), SDL_FLIP_NONE);

}

void Spell::Move() {
	y += dy;
	if (dy && !PosValidY()) {
		Kill();
		return;
	}
	
	x += dx;
	if (dx && !PosValidX()) {
		Kill();
		return;
	}
}
void Spell::Animate()
{
	frame_y = (state-1) * 15;

	
	frame_x += w;

	if (frame_x > last_frame[state])
	{
		if (state == SPELL_DESTROY)
		{
			exist = false;
			SDL_RemoveTimer(AnimTimerID);
			return;
		}
		frame_x = 0;
	}
}