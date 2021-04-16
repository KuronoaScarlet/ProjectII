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
#include "Title.h"
#include "DialogSystem.h"

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
	app->scene1->active = true;

	app->entityManager->AddEntity({ 500.0f, 175.0f }, Entity::Type::NPC1);
	app->entityManager->AddEntity({ 500.0f, 300.0f }, Entity::Type::NPC2);
	app->entityManager->AddEntity({ 400.0f, 525.0f }, Entity::Type::NPC3);
	
	app->entityManager->AddEntity({ app->playerPosition.x, app->playerPosition.y }, Entity::Type::PLAYER);

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

	SDL_Texture * texas = app->tex->Load("Assets/Fonts/kurale.png");
	font = new Fonts("Assets/Fonts/kurale.xml", texas);
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

	if (app->title->exi == true)
	{
		return false;
	}

	return true;
}

// Called each loop iteration
bool Scene1::PostUpdate()
{
	bool ret = true;
	time = 100 - timerr.ReadSec();

	if (app->entityManager->playerData.onDialog == true)
	{
		app->render->DrawRectangle({ 15,400,919,143 }, 255, 255, 150);

		char NPCdialogue[64] = { 0 };
		sprintf_s(NPCdialogue, 64, app->dialogueSystem->currentNode->text.c_str(), 56);
		app->render->DrawText(font, NPCdialogue, 336, 586, 50, 0, { 0, 0, 0, 255 });

		char response[64] = { 0 };
		for (int i = 0; i < app->dialogueSystem->currentNode->answersList.Count(); i++)
		{
			sprintf_s(response, 64, app->dialogueSystem->currentNode->answersList.At(i)->data.c_str(), 56);
			app->render->DrawText(font, response, 336, 586 + (50 * (i + 1)), 50, 0, { 0, 255, 255, 255 });
		}
	}

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
