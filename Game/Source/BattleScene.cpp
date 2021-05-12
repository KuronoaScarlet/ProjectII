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
#include "WinScreen.h"
#include "SceneManager.h"

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
	active = true;

	onTurn = false;
	playerTurn = false;
	allyTurn = false;
	attackMenu = false;
	defenseMenu = false;
	endTurn = false;
	attacked = false;
	defendOn = false;
	win = false;
	loose = false;


	//Paso 1: Cargar texturas, botones, música y Fx.
	app->audio->PlayMusic("Assets/Audio/Music/battleSong.ogg");
	screen = app->tex->Load("Assets/Textures/Screens/battle_scene.png");
	combatBox = app->tex->Load("Assets/Textures/combat_box.png");

	//BUTTONS---------------------------------------------------------
	attack = new GuiButton(101, { 230, 580, 216, 43 }, "attack");
	attack->SetObserver(this);
	attack->SetTexture(app->tex->Load("Assets/Textures/finger3.png"), app->tex->Load("Assets/Textures/finger4.png"), app->tex->Load("Assets/Textures/finger5.png"));

	defend = new GuiButton(102, { 480, 580, 216, 43 }, "defend");
	defend->SetObserver(this);
	defend->SetTexture(app->tex->Load("Assets/Textures/defend3.png"), app->tex->Load("Assets/Textures/defend4.png"), app->tex->Load("Assets/Textures/defend5.png"));

	run = new GuiButton(103, { 480, 640, 216, 43 }, "run");
	run->SetObserver(this);
	run->SetTexture(app->tex->Load("Assets/Textures/run3.png"), app->tex->Load("Assets/Textures/run4.png"), app->tex->Load("Assets/Textures/run5.png"));

	combine = new GuiButton(104, { 230, 640, 216, 43 }, "combine");
	combine->SetObserver(this);
	combine->SetTexture(app->tex->Load("Assets/Textures/combine3.png"), app->tex->Load("Assets/Textures/combine4.png"), app->tex->Load("Assets/Textures/combine5.png"));
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
	winScreen = app->tex->Load("Assets/Textures/battle_scene.png");

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
			
			if (pointer->type == Entity::Type::PLAYER)
			{
				if (attackMenu == false && defenseMenu == false)
				{
					sprintf_s(battleText, 64, "Que deberia hacer Player?");
					playerTurn = true;
					attack->Update(app->input, dt);
					run->Update(app->input, dt);
					defend->Update(app->input, dt);
					combine->Update(app->input, dt);
				}
				else if (attackMenu == true && endTurn == false)
				{
					if (rngEnemyNum == 0)
					{
						sprintf_s(battleText, 64, "Pulsa 1, 2 o 3 y ataca al enemigo!");
						if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
						{
							sprintf_s(battleText, 64, "Player ha atacado a Enemy 1!");
							DealDamage(pointer, app->entityManager->entityList.start->data, defendOn);
							endTurn = true;
						}
						if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
						{
							sprintf_s(battleText, 64, "Player ha atacado a Enemy 2!");
							DealDamage(pointer, app->entityManager->entityList.start->next->data, defendOn);
							endTurn = true;
						}
						if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
						{
							sprintf_s(battleText, 64, "Player ha atacado a Enemy 3!");
							DealDamage(pointer, app->entityManager->entityList.start->next->next->data, defendOn);
							endTurn = true;
						}
					}
					if (rngEnemyNum != 0)
					{
						sprintf_s(battleText, 64, "Pulsa 1 o 2 y ataca al enemigo!");
						if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
						{
							sprintf_s(battleText, 64, "Player ha atacado a Enemy 1!");
							DealDamage(pointer, app->entityManager->entityList.start->data, defendOn);
							endTurn = true;
						}
						if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
						{
							sprintf_s(battleText, 64, "Player ha atacado a Enemy 2!");
							DealDamage(pointer, app->entityManager->entityList.start->next->data, defendOn);
							endTurn = true;
						}
					}
				}
				else if (defenseMenu == true && endTurn == false)
				{
					sprintf_s(battleText, 64, "Player se ha defendido!");
					pointer->defending = true;
					endTurn = true;
				}
			}
			if (pointer->type == Entity::Type::ALLY1)
			{
				if (attackMenu == false && defenseMenu == false)
				{
					sprintf_s(battleText, 64, "Que deberia hacer Ally 1?");
					playerTurn = true;
					attack->Update(app->input, dt);
					run->Update(app->input, dt);
					defend->Update(app->input, dt);
					combine->Update(app->input, dt);
				}
				else if (attackMenu == true && endTurn == false)
				{
					if (rngEnemyNum == 0)
					{
						sprintf_s(battleText, 64, "Pulsa 1, 2 o 3 y ataca al enemigo!");
						if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
						{
							sprintf_s(battleText, 64, "Ally 1 ha atacado a Enemy 1!");
							DealDamage(pointer, app->entityManager->entityList.start->data, defendOn);
							endTurn = true;
						}
						if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
						{
							sprintf_s(battleText, 64, "Ally 1 ha atacado a Enemy 2!");
							DealDamage(pointer, app->entityManager->entityList.start->next->data, defendOn);
							endTurn = true;
						}
						if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
						{
							sprintf_s(battleText, 64, "Ally 1 ha atacado a Enemy 3!");
							DealDamage(pointer, app->entityManager->entityList.start->next->next->data, defendOn);
							endTurn = true;
						}
					}
					if (rngEnemyNum != 0)
					{
						sprintf_s(battleText, 64, "Pulsa 1 o 2 y ataca al enemigo!");
						if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
						{
							sprintf_s(battleText, 64, "Ally 1 ha atacado a Enemy 1!");
							DealDamage(pointer, app->entityManager->entityList.start->data, defendOn);
							endTurn = true;
						}
						if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
						{
							sprintf_s(battleText, 64, "Ally 1 ha atacado a Enemy 2!");
							DealDamage(pointer, app->entityManager->entityList.start->next->data, defendOn);
							endTurn = true;
						}
					}
				}
				else if (defenseMenu == true && endTurn == false)
				{
					sprintf_s(battleText, 64, "Ally 1 se ha defendido!");
					pointer->defending = true;
					endTurn = true;
				}
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
					sprintf_s(battleText, 64, "Enemy 2 ha atacado a un Aliado");
					DealDamage(pointer, app->entityManager->entityList.end->prev->data, app->entityManager->entityList.end->prev->data->defending);
					break;
				case 1: //Player//
					sprintf_s(battleText, 64, "Enemy 2 ha atacado al Player");
					DealDamage(pointer, app->entityManager->entityList.end->data, app->entityManager->entityList.end->prev->data->defending);
					break;
				}
				attacked = true;
			}
			if (pointer->type == Entity::Type::TANK_ENEMY)
			{
				switch (randomPick)
				{
				case 0: //Ally//
					sprintf_s(battleText, 64, "Enemy 1 ha atacado a un Aliado");
					DealDamage(pointer, app->entityManager->entityList.end->prev->data, app->entityManager->entityList.end->prev->data->defending);
					break;
				case 1: //Player//
					sprintf_s(battleText, 64, "Enemy 1 ha atacado al Player");
					DealDamage(pointer, app->entityManager->entityList.end->data, app->entityManager->entityList.end->prev->data->defending);
					break;
				}
				attacked = true;
			}
			if (pointer->type == Entity::Type::DAMAGE_ENEMY)
			{
				switch (randomPick)
				{
				case 0: //Ally//
					sprintf_s(battleText, 64, "Enemy 2 ha atacado a un Aliado");
					DealDamage(pointer, app->entityManager->entityList.end->prev->data, app->entityManager->entityList.end->prev->data->defending);
					break;
				case 1: //Player//
					sprintf_s(battleText, 64, "Enemy 2 ha atacado al Player");
					DealDamage(pointer, app->entityManager->entityList.end->data, app->entityManager->entityList.end->prev->data->defending);
					break;
				}
				attacked = true;
			}
		}
	}

	if (loose == true)
	{
		loose = false;
		app->sceneManager->ChangeScene(SCENE1);
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && win == false)
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
			if (remainingEnemies == 0)
			{
				win = true;
				app->entityManager->winCount++;
				if (app->entityManager->winCount == 3)
				{
					winScreenOnSceen = true;
					app->sceneManager->ChangeScene(WIN);
				}
			}
		}

		tmp = tmp->next;
	}

	if (win && app->entityManager->winCount != 3)
	{
		win = false;
		sprintf_s(battleText, 64, "Has ganado!");
		timerr.Start();
		counter = timerr.ReadSec();
		if (counter < 3)
		{
			app->sceneManager->ChangeScene(SCENE1);
		}
	}

	app->render->DrawTexture(screen, 0, 0, NULL);
	app->render->DrawTexture(combatBox, -app->render->camera.x, -app->render->camera.y + 530);
	PrintText();
	app->render->DrawText(app->render->font, battleText, 260, 550, 40, 3, { 0, 0, 0, 255 });

	if(playerTurn == true)
	{
		attack->Draw(app->render);
		run->Draw(app->render);
		defend->Draw(app->render);
		combine->Draw(app->render);
	}

	ListItem<Entity*>* bars = app->entityManager->entityList.start;

	int posy = 0;

	while (bars)
	{
		float barWidth = 75 * ((bars->data->turnTime - bars->data->combatTimer.counter) / bars->data->turnTime);
		SDL_Rect bar{ bars->data->position.x, posy, barWidth, 20 };
		posy += 40;
		app->render->DrawRectangle(bar, 128, 255, 128, 255, true, true);
		bars = bars->next;
	}

	return ret;
}

// Called before quitting
bool BattleScene::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();
	app->tex->UnLoad(screen);
	app->tex->UnLoad(combatBox);
	attack->CleanUp();
	defend->CleanUp();
	run->CleanUp();
	combine->CleanUp();
	active = false;

	LOG("Freeing scene");
	return true;
}

void BattleScene::ResumeCombat()
{
	onTurn = false;
	attacked = false;
	endTurn = false;
	attackMenu = false;
	defenseMenu = false;

	ListItem<Entity*>* tmp = app->entityManager->entityList.start;

	while (tmp)
	{
		tmp->data->combatTimer.Start();
		tmp = tmp->next;
	}
}

void BattleScene::DealDamage(Entity* attacker, Entity* deffender, bool defendOn)
{
	if(defendOn == false) deffender->currentHp = deffender->currentHp - attacker->atk;
	else if(defendOn == true) deffender->currentHp = deffender->currentHp - (attacker->atk - 20);
	deffender->defending = false;
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
				app->render->DrawText(app->render->font, num, 1070, (int)(550 * positionEnemy), 36, 3, { 0, 0, 0, 255 });
				app->render->DrawText(app->render->font, hp, 1100, (int)(580 * positionEnemy), 36, 3, { 0, 0, 0, 255 });
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
				app->render->DrawText(app->render->font, num, 80, (int)(550 * positionAlly), 36, 3, { 0, 0, 0, 255 });
				app->render->DrawText(app->render->font, hp, 110, (int)(580 * positionAlly), 36, 3, { 0, 0, 0, 255 });
				positionAlly += 0.1f;
			}

		}
		tmp = tmp->next;
	}
	
}