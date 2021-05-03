#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene12.h"
#include "Map.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Title.h"
#include "DialogSystem.h"
#include "HUD.h"
#include "Scene1.h"

#include "Defs.h"
#include "Log.h"

Scene12::Scene12() : Module()
{
	name.Create("scene");
}

// Destructor
Scene12::~Scene12()
{}

// Called before render is available
bool Scene12::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene12::Start()
{
	app->scene12->active = true;
	app->hud->Start();
	app->entityManager->AddEntity({ 32,416 }, Entity::Type::PLAYER);

	app->render->camera.y = 0;
	app->render->camera.x = 0;

	app->map->active = true;

	app->audio->PlayMusic("Assets/Audio/Music/scene1_music.ogg");

	if (app->loadingGame == true)
	{
		app->LoadGameRequest();
		app->loadingGame = false;
	}

	app->map->Load("mapLvl22.tmx");

	return true;
}

// Called each loop iteration
bool Scene12::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene12::Update(float dt)
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
bool Scene12::PostUpdate()
{
	bool ret = true;

	//if (app->entityManager->playerData.onDialog == true)
	//{

	//	char NPCdialogue[80] = { 0 };
	//	sprintf_s(NPCdialogue, 80, app->dialogueSystem->currentNode->text.c_str(), 56);
	//	app->render->DrawText(app->render->font, NPCdialogue, 150, 580, 45, 0, { 0, 0, 0, 255 });

	//	char response[80] = { 0 };
	//	for (int i = 0; i < app->dialogueSystem->currentNode->answersList.Count(); i++)
	//	{
	//		sprintf_s(response, 80, app->dialogueSystem->currentNode->answersList.At(i)->data.c_str(), 56);
	//		app->render->DrawText(app->render->font, response, 170, 600 + (25 * (i + 1)), 45, 0, { 255, 150, 150, 255 });
	//	}
	//}

	return ret;
}

// Called before quitting
bool Scene12::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	app->scene12->active = false;

	LOG("Freeing scene");
	return true;
}