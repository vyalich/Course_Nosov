#include "Dynamic.h"

void Dynamic::Move()
{
	x += dx;
	if (x > MAP_W_PX - w - 1)
		x = MAP_W_PX - w - 1;
	else if (x < 0)
		x = 0;
	
	PosValidX();

	y += dy;
	if (y > MAP_H_PX - h - 1)
		y = MAP_H_PX - h - 1;
	else if (y < 0)
		y = 0;

	PosValidY();
}

bool Dynamic::PosValidX()
{
	int ID;
	//double shift_x = 0;

	if (dx < 0) {
		ID = (int)(x + col_l) / TILE_SIZE + (int)(y + col_t) / TILE_SIZE * MAP_W;
	}
	else {
		ID = (int)(x - col_r + w) / TILE_SIZE + (int)(y + col_t) / TILE_SIZE * MAP_W;
	}
	for (int Y = (int)(y + col_t) / TILE_SIZE * TILE_SIZE; Y < y + h - col_b - 1; Y += TILE_SIZE) {
		if (Map::MapControl.GetTileType(ID) == TILE_BLOCK) {
			x = ID % MAP_W * TILE_SIZE;
			if (dx < 0)
				x += TILE_SIZE - col_l;
			else
				x -= w - col_r;
			//dx = 0;
			return false;
		}
		ID += MAP_W;
	}

	return true;
}

bool Dynamic::PosValidY()
{
	int ID;
	//double dy = 0;

	if (dy < 0) {
		ID = (int)(x + col_l) / TILE_SIZE + (int)(y + col_t) / TILE_SIZE * MAP_W;
	}
	else {
		ID = (int)(x + col_l) / TILE_SIZE + (int)(y + h) / TILE_SIZE * MAP_W;
	}
	for (int X = (int)(x + col_l) / TILE_SIZE * TILE_SIZE; X < x + w - col_r - 1; X += TILE_SIZE) {
		if (Map::MapControl.GetTileType(ID) == TILE_BLOCK) {
			y = ID / MAP_W * TILE_SIZE;
			if (dy < 0)
				y += TILE_SIZE - col_t;
			else
				y -= h - col_b;
			//dy = 0;
			return false;
		}
		ID++;
	}

	return true;
}

void Dynamic::Draw(SDL_Renderer* Render)
{
	if(onscreen)
		Drawer::Draw(Render, this->GetSpriteSheet(), x - Map::MapControl.GetX(), y - Map::MapControl.GetY(), frame_x, frame_y, w, h);
}

Uint32 Dynamic::AnimTimerCallback(Uint32 interval, void* data)
{
	Dynamic* obj = static_cast<Dynamic*>(data);

	obj->Animate();


	return interval;
}

void Dynamic::SetSpeed(double X, double Y)		//получаем мировые координаты цели
{
	if (state == BEING_IDLE)	//если объект не двигался начинаем анимацию движения
	{
		frame_x = 0;
		state = BEING_CHASE;
	}

	double way = sqrt((X - x) * (X - x) + (Y - y) * (Y - y));	//расчитываем путь до цели
	dx = speed / way * (X - x);	//считаем смещения вдоль осей, нормированное скоростью
	dy = speed / way * (Y - y);
	angle_cos = (X - x) / way;	//запоминаем угол поворота
}

void Dynamic::Stop()
{
	dx = dy = 0; 
	frame_x = 0; 
	state = BEING_IDLE;
}

void Dynamic::Animate()
{
	if (angle_cos > BOUNDCOS)
		direction = RIGHT;
	else if (angle_cos < -BOUNDCOS)
		direction = LEFT;
	else {
		if (dy < 0)
			direction = TOP;
		else if (dy > 0)
			direction = BOTTOM;
	}

	if(state == BEING_FLEE)
		frame_y = (BEING_CHASE * 4 + direction) * 64;
	else
		frame_y = (state * 4 + direction) * 64;

	frame_x += w;

	if (state == BEING_FLEE && frame_x > last_frame[BEING_CHASE])
		frame_x = 0;
	else if (frame_x > last_frame[state])
		frame_x = 0;
}