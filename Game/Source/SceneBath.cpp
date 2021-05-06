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
	app->sceneBath->active = true;
	app->hud->Start();
	app->playerPosition.x = 672.0f;
	app->playerPosition.y = 288.0f;
	app->entityManager->AddEntity({ app->playerPosition.x,app->playerPosition.y }, Entity::Type::PLAYER);

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

	if (app->title->exi == true)
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
