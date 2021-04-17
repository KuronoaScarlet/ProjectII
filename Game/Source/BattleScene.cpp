#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "BattleScene.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Title.h"
#include "DialogSystem.h"

#include "Defs.h"
#include "Log.h"
#include <time.h>

BattleScene::BattleScene() : Module()
{
	name.Create("battlescene");
}

// Destructor
BattleScene::~BattleScene()
{}

// Called before render is available
bool BattleScene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool BattleScene::Start()
{
	app->battleScene->active = true;

	//Paso 1: Cargar texturas, botones, m�sica y Fx.
	app->audio->PlayMusic("Assets/Audio/Music/battleSong.ogg");

	//BUTTONS---------------------------------------------------------
	attack = new GuiButton(1, { 517, 304, 250, 80 }, "attack");
	attack->SetObserver((Scene1*)this);
	attack->SetTexture(app->tex->Load("Assets/Textures/finger0.png"), app->tex->Load("Assets/Textures/finger1.png"), app->tex->Load("Assets/Textures/finger2.png"));

	defend = new GuiButton(1, { 800, 304, 250, 80 }, "defend");
	defend->SetObserver((Scene1*)this);
	defend->SetTexture(app->tex->Load("Assets/Textures/defend0.png"), app->tex->Load("Assets/Textures/defend1.png"), app->tex->Load("Assets/Textures/defend2.png"));

	run = new GuiButton(1, { 875, 400, 250, 80 }, "run");
	run->SetObserver((Scene1*)this);
	run->SetTexture(app->tex->Load("Assets/Textures/run0.png"), app->tex->Load("Assets/Textures/run1.png"), app->tex->Load("Assets/Textures/run2.png"));

	combine = new GuiButton(1, { 517, 400, 250, 80 }, "combine");
	combine->SetObserver((Scene1*)this);
	combine->SetTexture(app->tex->Load("Assets/Textures/combine0.png"), app->tex->Load("Assets/Textures/combine1.png"), app->tex->Load("Assets/Textures/combine2.png"));
	//--------------------------------------------------------------
	
	app->render->camera = { 0, 0 };

	//Paso 2: A�adir entidades enemigas. Random (2 o 3).
	int rngEnemyNum = rand() % 4;
	int rngTypeEnemy = rand() % 2;

	switch (rngEnemyNum)
	{
	case 0:
		app->entityManager->AddEntity({ 980.0f, 240.0f }, Entity::Type::TANK_ENEMY);
		app->entityManager->AddEntity({ 980.0f, 192.0f }, Entity::Type::DAMAGE_ENEMY);
		app->entityManager->AddEntity({ 980.0f, 288.0f }, Entity::Type::EQUILIBRATED_ENEMY);
		break;

	default:
		if (rngTypeEnemy == 0)
		{
			app->entityManager->AddEntity({ 1080.0f, 208.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 1080.0f, 272.0f }, Entity::Type::DAMAGE_ENEMY);
		}
		else if (rngTypeEnemy == 1)
		{
			app->entityManager->AddEntity({ 1080.0f, 192.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 1080.0f, 256.0f }, Entity::Type::EQUILIBRATED_ENEMY);
		}
		break;
	}

	//Paso 3: A�adir player y aliados (Animaci�n de Idle lateral "onFight").
	app->entityManager->AddEntity({ 280.0f, 192.0f }, Entity::Type::ALLY1);
	app->entityManager->AddEntity({ 280.0f, 256.0f }, Entity::Type::PLAYER);

	return true;
}

// Called each loop iteration
bool BattleScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool BattleScene::Update(float dt)
{
	/*attack->Update(app->input, dt);
	run->Update(app->input, dt);
	defend->Update(app->input, dt);
	combine->Update(app->input, dt);*/

	//L�GICA DEL BATTLE SYSTEM:
	//While que llama de manera permanente a las funciones de carga de la barra de turno.
	ListItem<Entity*>* tmp = app->entityManager->entityList.start;

	while (tmp)
	{
		tmp = tmp->next;
	}

	//Break del while cuando salte turno. Tener un puntero que apunte a la unidad que le toca turno.

	//Si es el player o un aliado:
	//Blitear texto, botones, funci�n recibe la acci�n en cuesti�n.
	//Si es enemigo:
	//A trav�s de randoms que se pickee una acci�n.

	//Funci�n para hacer perform de la acci�n (Texto + animaci�n (si da tiempo))
		
	//De vuelta al while.

	return true;
}

// Called each loop iteration
bool BattleScene::PostUpdate()
{
	SDL_Rect bg{ 0,0,1280,720 };
	app->render->DrawRectangle(bg, 255, 212, 2, 255, true, false);

	attack->Draw(app->render);
	run->Draw(app->render);
	defend->Draw(app->render);
	combine->Draw(app->render);
	
	bool ret = true;
	return ret;
}

// Called before quitting
bool BattleScene::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();

	app->battleScene->active = false;

	LOG("Freeing scene");
	return true;
}
