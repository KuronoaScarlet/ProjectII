#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene1.h"
#include "Map.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Pathfinding.h"
#include "Title.h"

#include "Defs.h"
#include "Log.h"

Scene1::Scene1() : Module()
{
	name.Create("scene");
}

// Destructor
Scene1::~Scene1()
{}

// Called before render is available
bool Scene1::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene1::Start()
{
	
	app->entityManager->AddEntity({ 100.0f, 175.0f }, Entity::Type::PLAYER);

	app->entityManager->AddEntity({ 500.0f, 175.0f }, Entity::Type::NPC_1);

	app->entityManager->AddEntity({ 500.0f, 300.0f }, Entity::Type::NPC_2);

	app->entityManager->AddEntity({ 400.0f, 525.0f }, Entity::Type::NPC_3);

	app->render->camera.y = 0;
	app->render->camera.x = 0;


	int w, h;
	uchar* data = NULL;

	//if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathFinding->SetMap(w, h, data);

	RELEASE_ARRAY(data);
	

	app->map->active = true;

	app->audio->PlayMusic("Assets/Audio/Music/scene1_music.ogg");

	if (app->loadingGame == true)
	{
		app->LoadGameRequest();
		app->loadingGame = false;
	}

	app->map->Load("mapLvl2.tmx");
	return true;
}

// Called each loop iteration
bool Scene1::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene1::Update(float dt)
{
	app->map->Draw();
	app->map->LoadColliders();

	return true;
}

// Called each loop iteration
bool Scene1::PostUpdate()
{
	bool ret = true;
	time = 100 - timerr.ReadSec();

	return ret;
}

// Called before quitting
bool Scene1::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	app->scene1->active = false;

	LOG("Freeing scene");
	return true;
}
