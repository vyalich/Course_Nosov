#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include "Map.h"
#include "Drawer.h"

class Entity
{
protected:
	double	x, y;
	char	w, h;
	bool	onscreen;

	virtual SDL_Texture* GetSpriteSheet() = 0;

public:
	Entity() {}
	virtual ~Entity() {}

	virtual void Draw(SDL_Renderer* Render) = 0;

	friend bool Compare(Entity*, Entity*);


	static std::list<Entity*>   OnScreen;
};

