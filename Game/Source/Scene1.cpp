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
#include "HUD.h"

#include "Defs.h"
#include "Log.h"

Scene1::Scene1() : Module()
{
	name.Create("scene1");

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
	app->hud->Start();
	app->entityManager->AddEntity({ 800.0f, 736.0f }, Entity::Type::NPC1);
	app->entityManager->AddEntity({ 352.0f, 1312.0f }, Entity::Type::NPC2);
	app->entityManager->AddEntity({ 1600.0f, 1024.0f }, Entity::Type::NPC2);
	app->entityManager->AddEntity({ 1568.0f, 192.0f }, Entity::Type::NPC2);
	app->entityManager->AddEntity({ 224.0f, 352.0f }, Entity::Type::NPC3);
	
	app->entityManager->AddEntity({ 192.0f, 416.0f}, Entity::Type::PENCIL);
	app->entityManager->AddEntity({ app->playerPosition.x, app->playerPosition.y }, Entity::Type::PLAYER);

	app->render->camera.y = 0;
	app->render->camera.x = 0;

	app->map->active = true;

	tp1to21 = app->collisions->AddCollider(SDL_Rect({ 992, 416, 32, 32 }), Collider::Type::TP1TO2, this);
	tp1to22 = app->collisions->AddCollider(SDL_Rect({ 992, 448, 32, 32 }), Collider::Type::TP1TO2, this);
	tp1to23 = app->collisions->AddCollider(SDL_Rect({ 992, 480, 32, 32 }), Collider::Type::TP1TO2, this);

	app->audio->PlayMusic("Assets/Audio/Music/scene1_music.ogg");

	if (app->loadingGame == true)
	{
		app->LoadGameRequest();
		app->loadingGame = false;
	}

	app->map->Load("mapLvl21.tmx");

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
	if (app->scene1->passingToLvl2)
	{
		app->fade->Fade((Module*)app->scene1, (Module*)app->scene12, 10);
	}
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

	if (app->entityManager->playerData.onDialog == true)
	{

		char NPCdialogue[80] = { 0 };
		sprintf_s(NPCdialogue, 80, app->dialogueSystem->currentNode->text.c_str(), 56);
		app->render->DrawText(app->render->font, NPCdialogue, 150, 580, 45, 0, { 0, 0, 0, 255 });

		char response[80] = { 0 };
		for (int i = 0; i < app->dialogueSystem->currentNode->answersList.Count(); i++)
		{
			sprintf_s(response, 80, app->dialogueSystem->currentNode->answersList.At(i)->data.c_str(), 56);
			app->render->DrawText(app->render->font, response, 170, 600 + (25 * (i + 1)), 45, 0, { 255, 150, 150, 255 });
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
