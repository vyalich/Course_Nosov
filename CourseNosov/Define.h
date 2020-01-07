#pragma once

#define MAP_H 100
#define MAP_W 140

#define MAP_H_PX 3200
#define MAP_W_PX 4480

#define WIN_H 720
#define WIN_W 1280

#define TILE_SIZE 32

#define BOUNDCOS 0.70710678118f

enum BEINGS_STATE
{
	BEING_IDLE = 0,	//�������� ����
	BEING_CHASE,		//�������� ����������
	BEING_ATTACK,		//�������� �������
	BEING_DEAD,			//�������� ������
	BEING_FLEE			//�������� �������
};

enum GAME_STATE
{
	GAME_RUNNING = 0,	//���� ������������
	GAME_OVER,			//���� ��������
	GAME_PAUSED			//���� ��������������
};

enum DIRECTION
{
	TOP = 0,
	LEFT,
	BOTTOM,
	RIGHT
};

enum TILE_TYPE
{
	TILE_FLOOR = 0,
	TILE_BLOCK,
	TILE_EXIT
};

enum FONTS
{
	FONT_ALTERNA_16 = 0
};

enum GUI_BARS
{
	GUI_HEALTH = 0,
	GUI_MANA,
	GUI_EXP
};

enum TEXT_ALLIGN
{
	
};

enum SPELL
{
	SPELL_CASTED = 1,
	SPELL_DESTROY
};

enum MAP_CREATOR
{
	DEADTOP = 8,
	DEADBOT = 4,
	BIRTH = 4,
	LOOPS = 8,
};