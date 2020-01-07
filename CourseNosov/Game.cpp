#include "Game.h"

bool Compare(Entity* A, Entity* B) {
	return A->y < B->y;
}

//-------------------------
int Game::Init()
{
	//инициализация сдл
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать SDL2. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать SDL 2, ошибка: %s", SDL_GetError());
		return -10;
	}

	atexit(SDL_Quit);

	//инициализация ттф
	if (TTF_Init() < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать TTF. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать TTF, ошибка: %s", TTF_GetError());
		return -20;
	}

	atexit(TTF_Quit);

	//инициализация имг
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно инициализировать TTF. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно инициализировать TTF, ошибка: %s", TTF_GetError());
		return -20;
	}

	atexit(IMG_Quit);

	//инициализация окна
	Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (Window == 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно создать окно. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно создать окно, ошибка: %s", SDL_GetError());
		return -30;
	}

	//создание рендера для окна
	Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	if (Render == NULL)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"Ошибка", u8"Невозможно создать рендерер. Посмотрите лог для подробной информации.", NULL);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, u8"Невозможно создать рендерер, ошибка: %s", SDL_GetError());
		return -40;
	}

	//загрузка шрифтов
	if (Drawer::LoadFonts() < 0)
		return -50;

	//загрузка тайлеста карты
	if (Map::LoadStatic(Render, "tileset.png") < 0)
		return -60;

	//иинициализация объекта игрока
	if (Player::player.Load("player.png", Render) < 0)
		return -70;

	//загрузка тайлсета врагов
	if (Enemy::LoadStatic(Render) < 0)
		return -80;

	//загрузка GUI
	if (GUI::GUIControl.Load(Render) < 0)
		return -90;

	//загрузка спелов
	if (Spell::LoadStatic(Render) < 0)
		return -100;

	//Initialize renderer color
	SDL_SetRenderDrawColor(Render, 0, 0, 0, 0xFF);

	floor = 0;

	srand(time(0));

	

	return 0;
}

int Game::LevelInit()
{
	//инициализация карты
	Map::MapControl.Load();


	_level = true;

	floor++;

	enemy_cnt = 20 * floor;

	Player::player.LevelInit();

	Entity::OnScreen.push_back(&Player::player);
	 
	//инициализация врагов
	Enemy* temp;
	for (int i = 0; i < enemy_cnt; i++) {
		temp = new Enemy;
		Enemy::EnemyList.push_back(temp);
		Enemy::EnemyList[i]->Load();
	}

	return 0;
}

void Game::LevelClear()
{
	Map::MapControl.ClearLevel();

	Entity::OnScreen.clear();

	for (int i = 0; i < enemy_cnt; i++)
	{
		Enemy::EnemyList[i]->Clear();
		delete Enemy::EnemyList[i];
	}
	Enemy::EnemyList.clear();

	for (auto spell : Spell::SpellCasted)
	{
		delete spell;
	}

	Spell::SpellCasted.clear();
}


//-------------------------
void Game::Clear()
{
	Map::MapControl.Clear();
	TTF_Quit();
	SDL_Quit();
}


//-------------------------
int Game::MainCycle()
{
	if (Init() < 0)
		return -100;

	

	SDL_Event e;
	
	while (_running == GAME_RUNNING)
	{
		LevelInit();

		MainCycleTimerID = SDL_AddTimer(1000 / 60, MainCycleTimerCallback, this);

		while (_level) {
			while (SDL_PollEvent(&e))
			{
				Handle(&e);
			}

		}

		SDL_RemoveTimer(MainCycleTimerID);

		LevelClear();
	}

	Clear();
}


//-------------------------
Uint32 Game::MainCycleTimerCallback(Uint32 interval, void* data)
{
	Game* game_obj = static_cast<Game*>(data);

	if(game_obj->Process())
		game_obj->Draw();

	return interval;
}


//-------------------------
void Game::Handle(SDL_Event* e)
{
	switch (e->type)
	{
	case SDL_QUIT:
		_running = GAME_OVER;
		_level = false;
		break;

	case SDL_KEYDOWN:
		switch (e->key.keysym.sym)
		{
		case SDLK_f:	_level = false; SDL_RemoveTimer(MainCycleTimerID);
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (e->button.button == SDL_BUTTON_LEFT)
		{
			Player::player.SetSpeed(e->button.x + Map::MapControl.GetX() - 32, e->button.y + Map::MapControl.GetY() - 32);
		}
		if (e->button.button == SDL_BUTTON_RIGHT)
		{
			Spell::AddCasted(e->button.x, e->button.y);
		}
		break;

	case SDL_MOUSEMOTION:
		if (e->button.button == SDL_BUTTON_LEFT)
		{
			Player::player.SetSpeed(e->button.x + Map::MapControl.GetX() - 32, e->button.y + Map::MapControl.GetY() - 32);
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (e->button.button == SDL_BUTTON_LEFT)
		{
			Player::player.Stop();
		}
		break;
	}
}


//-------------------------
bool Game::Process()
{
	if (!Player::player.Process())
	{
		_level = false;
		if (!Player::player.Alive())
			_running = GAME_OVER;
		SDL_RemoveTimer(MainCycleTimerID);
		return false;

	}

	double TarX = Player::player.GetX();
	double TarY = Player::player.GetY();
	int CamX = Map::MapControl.GetX();
	int CamY = Map::MapControl.GetY();

	//Map::MapControl.Draw(Render);

	for (int i = 0; i < enemy_cnt; i++)
		Enemy::EnemyList[i]->Process(TarX, TarY, CamX, CamY, Render);

	for (auto spell = Spell::SpellCasted.begin(); spell != Spell::SpellCasted.end();)
	{
		(*spell)->Process();
		if (!(*spell)->Exist())
		{
			delete (*spell);
			spell = Spell::SpellCasted.erase(spell);
		}
		else
			spell++;
	}

	Entity::OnScreen.sort(Compare);

	return true;
}


//-------------------------
void Game::Draw()
{
	Map::MapControl.Draw(Render);

	for (auto entity : Entity::OnScreen) {
		entity->Draw(Render);
	}

	for (auto spell : Spell::SpellCasted) {
		spell->Draw(Render, Map::MapControl.GetX(), Map::MapControl.GetY());
	}

	GUI::GUIControl.Draw(Render);

	SDL_RenderPresent(Render);
}

int main(int argc, char* argv[])
{
	Game game;
	return game.MainCycle();
}