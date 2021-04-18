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
#include <stdlib.h>


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

	//Paso 1: Cargar texturas, botones, música y Fx.
	app->audio->PlayMusic("Assets/Audio/Music/battleSong.ogg");
	screen = app->tex->Load("Assets/Textures/Screens/battle_scene.png");

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

	//Paso 2: Añadir entidades enemigas. Random (2 o 3).
	rngEnemyNum = rand() % 4;
	rngTypeEnemy = rand() % 2;

	switch (rngEnemyNum)
	{
	case 0:
		app->entityManager->AddEntity({ 880.0f, 336.0f }, Entity::Type::TANK_ENEMY);
		app->entityManager->AddEntity({ 880.0f, 288.0f }, Entity::Type::DAMAGE_ENEMY);
		app->entityManager->AddEntity({ 880.0f, 384.0f }, Entity::Type::EQUILIBRATED_ENEMY);
		remainingEnemies = 3;
		break;

	default:
		if (rngTypeEnemy == 0)
		{
			app->entityManager->AddEntity({ 880.0f, 304.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::DAMAGE_ENEMY);
			remainingEnemies = 2;
		}
		else if (rngTypeEnemy == 1)
		{
			app->entityManager->AddEntity({ 880.0f, 304.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::EQUILIBRATED_ENEMY);
			remainingEnemies = 2;
		}
		break;
	}

	//Paso 3: Añadir player y aliados (Animación de Idle lateral "onFight").
	app->entityManager->AddEntity({ 280.0f, 304.0f }, Entity::Type::ALLY1);
	app->entityManager->AddEntity({ 280.0f, 368.0f }, Entity::Type::PLAYER);
	remainingAllies = 2;

	//Paso 4: Start de los Timers de Turno.
	ResumeCombat();

	SDL_Texture* texas = app->tex->Load("Assets/Fonts/kurale.png");
	font = new Fonts("Assets/Fonts/kurale.xml", texas);

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
	//LÓGICA DEL BATTLE SYSTEM:
	//While que llama de manera permanente a las funciones de carga de la barra de turno.
	ListItem<Entity*>* tmp = app->entityManager->entityList.start;
	
	if (onTurn == false)
	{
		while (tmp)
		{
			if (tmp->data->SpeedCounter() == true)
			{
				pointer = tmp->data;
				onTurn = true;
				ListItem<Entity*>* tmp2 = app->entityManager->entityList.start;
				while (tmp2)
				{
					tmp2->data->combatTimer.Stop();
					tmp2 = tmp2->next;
				}

				break;
			}
			tmp = tmp->next;
		}
	}

	if (onTurn == true && attacked == false)
	{
		switch (pointer->collider->type)
		{
		case Collider::Type::PLAYER:
			/*attack->Update(app->input, dt);
			run->Update(app->input, dt);
			defend->Update(app->input, dt);
			combine->Update(app->input, dt);*/
			if (pointer->type == Entity::Type::PLAYER)
			{
				app->render->DrawRectangle(SDL_Rect{ 1000, 300, 20, 20 }, 0, 255, 0, 255, true, false);
			}
			if (pointer->type == Entity::Type::ALLY1)
			{
				app->render->DrawRectangle(SDL_Rect{ 1000, 300, 20, 20 }, 255, 0, 255, 255, true, false);
			}
			
			break;

		case Collider::Type::ENEMY:
			int randomPick = rand() % 2;
			if (app->entityManager->entityList.end->prev->data->type != Entity::Type::ALLY1)
			{
				randomPick = 1;
			}

			if (pointer->type == Entity::Type::EQUILIBRATED_ENEMY)
			{
				switch (randomPick)
				{
				case 0: //Ally//
					sprintf_s(battleText, 64, "Enemigo 2 ha atacado a un Aliado");
					DealDamage(pointer, app->entityManager->entityList.end->prev->data);
					break;
				case 1: //Player//
					sprintf_s(battleText, 64, "Enemigo 2 ha atacado al Player");
					DealDamage(pointer, app->entityManager->entityList.end->data);
					break;
				}
				attacked = true;
			}
			if (pointer->type == Entity::Type::TANK_ENEMY)
			{
				switch (randomPick)
				{
				case 0: //Ally//
					sprintf_s(battleText, 64, "Enemigo 1 ha atacado a un Aliado");
					DealDamage(pointer, app->entityManager->entityList.end->prev->data);
					break;
				case 1: //Player//
					sprintf_s(battleText, 64, "Enemigo 1 ha atacado al Player");
					DealDamage(pointer, app->entityManager->entityList.end->data);
					break;
				}
				attacked = true;
			}
			if (pointer->type == Entity::Type::DAMAGE_ENEMY)
			{
				switch (randomPick)
				{
				case 0: //Ally//
					sprintf_s(battleText, 64, "Enemigo 2 ha atacado a un Aliado");
					DealDamage(pointer, app->entityManager->entityList.end->prev->data);
					break;
				case 1: //Player//
					sprintf_s(battleText, 64, "Enemigo 2 ha atacado al Player");
					DealDamage(pointer, app->entityManager->entityList.end->data);
					break;
				}
				attacked = true;
			}
		}
	}

	if (loose == true)
	{
		loose = false;
		app->fade->Fade(this, (Module*)app->scene1);
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ResumeCombat();
	}

	return true;
}

// Called each loop iteration
bool BattleScene::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* tmp = app->entityManager->entityList.start;
	
	while (tmp)
	{
		if (tmp->data->currentHp <= 0)
		{
			switch (tmp->data->collider->type)
			{
			case Collider::Type::PLAYER:
				remainingAllies--;
				break;
			case Collider::Type::ENEMY:
				remainingEnemies--;
			}
			tmp->data->dead = true;
			tmp->data->pendingToDelete = true;

			if (remainingAllies == 0)
			{
				loose = true;
			}
		}
		tmp = tmp->next;
	}

	app->render->DrawTexture(screen, 0, 0, NULL);
	app->render->DrawRectangle({ -app->render->camera.x,-app->render->camera.y + 530,1280,250 }, 255, 255, 150);
	PrintText();
	app->render->DrawText(app->render->font, battleText, 300, 560, 40, 3, { 0, 0, 0, 255 });




	/*attack->Draw(app->render);
	run->Draw(app->render);
	defend->Draw(app->render);
	combine->Draw(app->render);*/
	
	return ret;
}

// Called before quitting
bool BattleScene::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->tex->UnLoad(screen);

	app->battleScene->active = false;

	LOG("Freeing scene");
	return true;
}

void BattleScene::ResumeCombat()
{
	onTurn = false;
	attacked = false;

	ListItem<Entity*>* tmp = app->entityManager->entityList.start;

	while (tmp)
	{
		tmp->data->combatTimer.Start();
		tmp = tmp->next;
	}
}

void BattleScene::DealDamage(Entity* attacker, Entity* deffender)
{
	deffender->currentHp = deffender->currentHp - attacker->atk;
}

void BattleScene::PrintText()
{
	ListItem<Entity*>* tmp = app->entityManager->entityList.start;
	int countAlly = 1;
	int countEnemy = 1;
	float positionEnemy = 1;
	float positionAlly = 1;


	while (tmp)
	{
		if (tmp->data->currentHp > 0)
		{
			sprintf_s(hp, 10, "%d/%d", tmp->data->currentHp, tmp->data->hp);
			if (tmp->data->collider->type == Collider::Type::ENEMY)
			{
				sprintf_s(num, 10, "Enemy %d", countEnemy);
				app->render->DrawText(app->render->font, num, 1070, (int)(560 * positionEnemy), 36, 3, { 0, 0, 0, 255 });
				app->render->DrawText(app->render->font, hp, 1100, (int)(590 * positionEnemy), 36, 3, { 0, 0, 0, 255 });
				positionEnemy += 0.1f;
				countEnemy++;
			}
			
			if (tmp->data->collider->type == Collider::Type::PLAYER)
			{
				if (tmp->data->type == Entity::Type::PLAYER)
				{
					sprintf_s(num, 10, "Player");
				}
				else if (tmp->data->type != Entity::Type::PLAYER)
				{
					sprintf_s(num, 10, "Ally: %d", countAlly);
					countAlly++;
				}
				app->render->DrawText(app->render->font, num, 45, (int)(560 * positionAlly), 36, 3, { 0, 0, 0, 255 });
				app->render->DrawText(app->render->font, hp, 75, (int)(590 * positionAlly), 36, 3, { 0, 0, 0, 255 });
				positionAlly += 0.1f;
			}

		}
		tmp = tmp->next;
	}
	
}

void BattleScene::BattleText(Entity* attacker, Entity* deffender)
{

}