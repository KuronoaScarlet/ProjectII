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
#include "Scene12.h"
#include "SceneManager.h"
#include "ParticlesEngine.h"

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
	//app->scene12->lpl = false;
	passingToLvl2 = false;
	active = true;
	app->hud->Start();
	app->entityManager->AddEntity({app->playerPosition.x, app->playerPosition.y }, Entity::Type::PLAYER);

	app->sceneManager->scenebath = false;
	app->sceneManager->scenegym = false;

	app->entityManager->AddEntity({ 800.0f, 736.0f }, Entity::Type::NPC1);
	app->entityManager->AddEntity({ 352.0f, 1312.0f }, Entity::Type::NPC2);
	app->entityManager->AddEntity({ 1600.0f, 1024.0f }, Entity::Type::NPC2);
	app->entityManager->AddEntity({ 1568.0f, 192.0f }, Entity::Type::NPC2);
	app->entityManager->AddEntity({ 224.0f, 352.0f }, Entity::Type::NPC3);
	app->entityManager->AddEntity({ 480.0f, 1248.0f }, Entity::Type::NPCQ2);
	app->entityManager->AddEntity({ 256.0f, 992.0f }, Entity::Type::NPCQ3);
	
	app->entityManager->AddEntity({ 192.0f, 416.0f}, Entity::Type::PENCIL);
	app->entityManager->AddEntity({ 220.0f, 1400.0f}, Entity::Type::COFFEE);
	app->entityManager->AddEntity({ 90.0f, 1445.0f}, Entity::Type::CAN);
	app->entityManager->AddEntity({ 96.0f, 128.0f }, Entity::Type::CAN);
	app->entityManager->AddEntity({ 320.0f,128.0f}, Entity::Type::BALL);
	app->entityManager->AddEntity({ 736.0f, 128.0f}, Entity::Type::CALCULATOR);	
	
	
	app->entityManager->AddEntity({ 100.0f, 1445.0f}, Entity::Type::BOOK);
	app->entityManager->AddEntity({ 100.0f, 1445.0f}, Entity::Type::RUBBER);
	app->entityManager->AddEntity({ 100.0f, 1445.0f}, Entity::Type::RULE);
	
	
	

	app->render->camera.y = 0;
	app->render->camera.x = 0;

	app->map->active = true;

	//Up
	tp1to21 = app->collisions->AddCollider(SDL_Rect({ 1024, 416, 32, 96 }), Collider::Type::TP1TO2, this);

	//Down
	tp1to21 = app->collisions->AddCollider(SDL_Rect({ 992, 1056, 32, 96 }), Collider::Type::TP1TO2, this);
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

	if (passingToLvl2 == true && lpl == false)
	{
		app->sceneManager->ChangeScene(SCENE12,0);
		lpl = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->sceneManager->ChangeScene(BATH,0);
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->sceneManager->ChangeScene(GYM,0);
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

	active = false;

	LOG("Freeing scene");
	return true;
}
