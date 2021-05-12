#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneGym.h"
#include "Map.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Title.h"
#include "DialogSystem.h"
#include "HUD.h"
#include "Scene12.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

SceneGym::SceneGym() : Module()
{
	name.Create("scene1");

}

// Destructor
SceneGym::~SceneGym()
{}

// Called before render is available
bool SceneGym::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneGym::Start()
{
	active = true;
	app->hud->Start();

	app->playerPosition.x = 672.0f;
	app->playerPosition.y = 288.0f;

	app->entityManager->AddEntity({32, 96 }, Entity::Type::PLAYER);

	app->entityManager->AddEntity({ 288, 200 }, Entity::Type::ENEMYLANTERN2);
	app->entityManager->AddEntity({ 448, 32 }, Entity::Type::ENEMYLANTERN2);

	goright = app->collisions->AddCollider(SDL_Rect({ 32, 224, 10, 10 }), Collider::Type::GORIGHT, this);
	goleft = app->collisions->AddCollider(SDL_Rect({ 352, 224, 10, 10 }), Collider::Type::GOLEFT, this);
	godown1 = app->collisions->AddCollider(SDL_Rect({ 448, 32, 10, 10 }), Collider::Type::GODOWN, this);
	goup = app->collisions->AddCollider(SDL_Rect({ 448, 448, 10, 10 }), Collider::Type::GOUP, this);

	app->render->camera.y = 0;
	app->render->camera.x = 0;

	app->map->active = true;

	if (app->loadingGame == true)
	{
		app->LoadGameRequest();
		app->loadingGame = false;
	}

	app->map->Load("GymDungeon.tmx");

	return true;
}

// Called each loop iteration
bool SceneGym::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneGym::Update(float dt)
{
	app->map->Draw();
	app->map->LoadColliders();



	return true;
}

// Called each loop iteration
bool SceneGym::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneGym::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	active = false;

	LOG("Freeing scene");
	return true;
}
