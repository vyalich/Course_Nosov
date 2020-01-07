#pragma once

#include "Define.h"
#include "Drawer.h"
#include "Dynamic.h"
#include "Player.h"
#include "Enemy.h"


class Spell : public Dynamic{
	private:
		bool                    exist;
		
		int						Power;
		double                  TarX;
		double                  TarY;
		static  int             Cost;
		static Uint16           CoolDown;
		static Uint16           Range;
		static Uint32           LastUsed;;
		static SDL_Texture* sprite_sheet;

		SDL_Texture* GetSpriteSheet() { return Spell::sprite_sheet; }

	public:
		static std::list<Spell*> SpellCasted;


		Spell() {}

		bool Exist() { return exist; }

		void Load();
		
		void Process();
		
		void Draw(SDL_Renderer* Render, int CamX, int CamY);
		
		void Move();

		void Kill();

		void Animate() override;

		static int LoadStatic(SDL_Renderer* Render);

		static void AddCasted(double t_x, double t_y);

};

