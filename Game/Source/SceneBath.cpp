#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneBath.h"
#include "Map.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Title.h"
#include "DialogSystem.h"
#include "HUD.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

SceneBath::SceneBath() : Module()
{
	name.Create("sceneBath");

}

// Destructor
SceneBath::~SceneBath()
{}

// Called before render is available
bool SceneBath::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneBath::Start()
{
	active = true;
	app->hud->Start();
	app->playerPosition.x = 320.0f;
	app->playerPosition.y = 768.0f;
	app->entityManager->AddEntity({ app->playerPosition.x,app->playerPosition.y }, Entity::Type::PLAYER);
	app->entityManager->AddEntity({ 640,384 }, Entity::Type::ENEMYLANTERN1);
	app->entityManager->AddEntity({ 288,640 }, Entity::Type::ENEMYLANTERN1);
	app->entityManager->AddEntity({ 544,576 }, Entity::Type::ENEMYLANTERN2);

	godown1 = app->collisions->AddCollider(SDL_Rect({ 310, 600, 10, 10 }), Collider::Type::GODOWN, this);
	goup = app->collisions->AddCollider(SDL_Rect({ 672, 736, 10, 10 }), Collider::Type::GOUP, this);
	goright = app->collisions->AddCollider(SDL_Rect({ 340, 768, 10, 10 }), Collider::Type::GORIGHT, this);
	goleft = app->collisions->AddCollider(SDL_Rect({ 672, 576, 10, 10 }), Collider::Type::GOLEFT, this);

	app->render->camera.y = 0;
	app->render->camera.x = 0;

	app->map->active = true;

	if (app->loadingGame == true)
	{
		app->LoadGameRequest();
		app->loadingGame = false;
	}

	app->map->Load("BathDungeon.tmx");

	return true;
}

// Called each loop iteration
bool SceneBath::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneBath::Update(float dt)
{
	app->map->Draw();
	app->map->LoadColliders();

	if (app->sceneManager->exi == true)
	{
		return false;
	}


	return true;
}

// Called each loop iteration
bool SceneBath::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool SceneBath::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	LOG("Freeing scene");
	return true;
}