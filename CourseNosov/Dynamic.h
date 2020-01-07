#pragma once

#include "Entity.h"
#include "Define.h"

class Dynamic: public Entity
{
protected:
	double		speed;	//скорость
	double		dx, dy;	//смещения вдоль осей координат
	char		col_l, col_t, col_r, col_b;	

	char		state;
	char		direction;		//направление движения
	double		angle_cos;		//угол поворота
	short		last_frame[4];		//последний кадр для текущей анимации
	short		frame_x;		//координаты кадра из тайлсета
	short		frame_y;		//
	SDL_TimerID AnimTimerID;

	

	void Move();

	bool PosValidX();

	bool PosValidY();

	virtual void Animate();

	virtual SDL_Texture* GetSpriteSheet() = 0;

	static Uint32 AnimTimerCallback(Uint32 interval, void* data);

public:
	Dynamic() {}
	virtual ~Dynamic() {}

	virtual void Process() {}

	void Draw(SDL_Renderer* Render);

	virtual void Clear() {}

	void SetSpeed(double X, double Y);

	void Stop();
};

