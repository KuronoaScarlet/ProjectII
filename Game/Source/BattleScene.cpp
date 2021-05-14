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
	turn = UNKNOWN;
	state = WAITING;

	skipBarMax = { 0, 720, 100, 16 };
	skipBar = { 0, 721, 0, 14 };

	//Paso 1: Cargar texturas, botones, música y Fx.
	app->audio->PlayMusic("Assets/Audio/Music/battleSong.ogg");
	screen = app->tex->Load("Assets/Textures/Screens/battle_scene.png");
	combatBox = app->tex->Load("Assets/Textures/combat_box.png");

	//BUTTONS---------------------------------------------------------
	attack = new GuiButton(101, { 280, 590, 216, 43 }, "attack");
	attack->SetObserver(this);
	attack->SetTexture(app->tex->Load("Assets/Textures/finger3.png"), app->tex->Load("Assets/Textures/finger4.png"), app->tex->Load("Assets/Textures/finger5.png"));

	defend = new GuiButton(102, { 530, 590, 216, 43 }, "defend");
	defend->SetObserver(this);
	defend->SetTexture(app->tex->Load("Assets/Textures/defend3.png"), app->tex->Load("Assets/Textures/defend4.png"), app->tex->Load("Assets/Textures/defend5.png"));

	run = new GuiButton(103, { 530, 660, 216, 43 }, "run");
	run->SetObserver(this);
	run->SetTexture(app->tex->Load("Assets/Textures/run3.png"), app->tex->Load("Assets/Textures/run4.png"), app->tex->Load("Assets/Textures/run5.png"));

	combine = new GuiButton(104, { 280, 660, 216, 43 }, "combine");
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
		app->entityManager->AddEntity({ 880.0f, 288.0f }, Entity::Type::TANK_ENEMY);
		app->entityManager->AddEntity({ 960.0f, 328.0f }, Entity::Type::DAMAGE_ENEMY);
		app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::EQUILIBRATED_ENEMY);
		remainingEnemies = 3;
		break;

	default:
		if (rngTypeEnemy == 0)
		{
			app->entityManager->AddEntity({ 880.0f, 288.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::DAMAGE_ENEMY);
			remainingEnemies = 2;
		}
		else if (rngTypeEnemy == 1)
		{
			app->entityManager->AddEntity({ 880.0f, 288.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::EQUILIBRATED_ENEMY);
			remainingEnemies = 2;
		}
		break;
	}

	//Paso 3: Añadir player y aliados (Animación de Idle lateral "onFight").
	app->entityManager->AddEntity({ 280.0f, 288.0f }, Entity::Type::PLAYER);
	app->entityManager->AddEntity({ 280.0f, 368.0f }, Entity::Type::ALLY1);
	remainingAllies = 2;

	

	SDL_Texture* texas = app->tex->Load("Assets/Fonts/kurale.png");
	winScreen = app->tex->Load("Assets/Textures/battle_scene.png");

	app->sceneManager->CreateQuest(3, "win a combat");

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
	//app->entityManager->Update(dt);
	//While que llama de manera permanente a las funciones de carga de la barra de turno.
	ListItem<Entity*>* tmp = app->entityManager->entityList.start;

	if (onTurn == false)
	{
		while (tmp)
		{
			tmp->data->TurnBarUpdate();
			if (tmp->data->TurnBarUpdate() == true)
			{
				onTurn = true;
				turnEntity = tmp->data;

				if (turnEntity->collider->type == Collider::Type::PLAYER) turn = PLAYER_TURN;
				else if (turnEntity->collider->type == Collider::Type::ENEMY) turn = ENEMY_TURN;

				break;
			}
			tmp = tmp->next;
		}

	}

	PerformCombat(dt);

	if (state == SELECT_ACTION || state == PERFORM_ACTION || state == FINISH_TURN)
	{
		skipBar.w++;

		if (skipBar.w > (skipBarMax.w))
		{
			skipBar.w = (skipBarMax.w - 1);
		}
	}


	return true;
}

// Called each loop iteration
bool BattleScene::PostUpdate()
{
	bool ret = true;


	app->render->DrawTexture(screen, 0, 0, NULL);
	app->render->DrawTexture(combatBox, -app->render->camera.x, -app->render->camera.y + 530);
	PrintText();
	app->render->DrawText(app->render->font, battleText, 240, 530, 50, 3, { 255, 255, 255, 255 });

	if (turn == PLAYER_TURN && state == SELECT_ACTION && app->sceneManager->atkMenu == false && app->sceneManager->defMenu == false)
	{
		attack->Draw(app->render);
		run->Draw(app->render);
		defend->Draw(app->render);
		combine->Draw(app->render);
	}

	app->render->DrawRectangle(skipBarMax, 0, 0, 0, 255);
	app->render->DrawRectangle(skipBar, 0, 0, 255, 255);

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

void BattleScene::PerformCombat(float dt)
{
	switch (state)
	{
	case WAITING: state = SELECT_ACTION;
		break;

	case SELECT_ACTION:
		if (turn == PLAYER_TURN)
		{
			if (app->sceneManager->atkMenu == false && app->sceneManager->defMenu == false) ShowMenu(dt);

			// ATAQUE
			else if (app->sceneManager->atkMenu == true && app->sceneManager->defMenu == false)
			{
				enemySelection = SelectEnemy(remainingEnemies);
				skipBar.w = 0;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// DEFENSA
			else if (app->sceneManager->atkMenu == false && app->sceneManager->defMenu == true)
			{
				turnEntity->defending = true;
				skipBar.w = 0;
				state = PERFORM_ACTION;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else if (turn == ENEMY_TURN)
		{
			sprintf_s(battleText, 64, "El enemigo piensa que hacer...");
			if (skipBar.w == skipBarMax.w)
			{
				allySelection = SelectAlly(remainingAllies);
				skipBar.w = 0;
				state = PERFORM_ACTION;
			}
		}
		break;

	case PERFORM_ACTION:
		if (turn == PLAYER_TURN)
		{
			if (turnEntity->defending == true) sprintf_s(battleText, 64, "Bloqueando ataques!");
			else if (turnEntity->defending == false) sprintf_s(battleText, 64, "Atacando! ");
		}
		else if (turn == ENEMY_TURN)
		{
			sprintf_s(battleText, 64, "El enemigo ataca!");
		}

		//Aquí irán las animaciones en general (Ataques, curas...)
		if (skipBar.w == skipBarMax.w)
		{
			if (turn == PLAYER_TURN && turnEntity->defending == false) DealDamage(turnEntity, app->entityManager->entityList.At(enemySelection)->data);
			else if (turn == ENEMY_TURN)
			{
				DealDamage(turnEntity, app->entityManager->entityList.At(remainingEnemies + allySelection)->data);
			}
			skipBar.w = 0;
			state = FINISH_TURN;
		}
		break;

	case FINISH_TURN:
		//Comprovar cuantos enemigos o aliados quedan vivos.
		tmp = app->entityManager->entityList.start;
		while (tmp)
		{
			if (tmp->data->currentHp <= 0)
			{
				tmp->data->pendingToDelete = true;
				if (tmp->data->collider->type == Collider::Type::ENEMY) remainingEnemies--;
				else if (tmp->data->collider->type == Collider::Type::PLAYER) remainingAllies--;
			}
			tmp = tmp->next;
		}

		if (skipBar.w == skipBarMax.w)
		{
			skipBar.w = 0;

			//Si quedan de ambos, ResumeCombat();
			if (remainingAllies != 0 && remainingEnemies != 0) ResumeCombat();

			//Si no, función para enviar a Victory o Loose State
			if (remainingAllies == 0 || remainingEnemies == 0)
			{
				app->sceneManager->winn = true;
				app->sceneManager->ChangeScene(SCENE1, 0);
			}
		}
		break;

	default:
		break;
	}
}

void BattleScene::ShowMenu(float dt)
{
	if (turnEntity->type == Entity::Type::PLAYER) sprintf_s(battleText, 64, "Que deberia hacer Player?");
	if (turnEntity->type == Entity::Type::ALLY1) sprintf_s(battleText, 64, "Que deberia hacer Ally 1?");
	attack->Update(app->input, dt);
	defend->Update(app->input, dt);
	combine->Update(app->input, dt);
	run->Update(app->input, dt);

}

int BattleScene::SelectEnemy(int enemyNum)
{
	turnEntity->defending = false;

	switch (enemyNum)
	{
	case 3:
		sprintf_s(battleText, 64, "Pulsa 1, 2 o 3 y ataca al enemigo!");
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
		{
			state = PERFORM_ACTION;
			return 0;
		}
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
		{
			state = PERFORM_ACTION;
			return 1;
		}
		if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
		{
			state = PERFORM_ACTION;
			return 2;
		}
		break;
	case 2:
		sprintf_s(battleText, 64, "Pulsa 1 o 2 y ataca al enemigo!");
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
		{
			state = PERFORM_ACTION;
			return 0;
		}
		if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && app->entityManager->entityList.start->next->data->collider->type != Collider::Type::PLAYER)
		{
			state = PERFORM_ACTION;
			return 1;
		}
		break;
	case 1:
		sprintf_s(battleText, 64, "Pulsa 1 y ataca al enemigo!");
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			state = PERFORM_ACTION;
			return 0;
		}
		break;
	default:
		break;
	}
}

int BattleScene::SelectAlly(int allyNum)
{
	switch (allyNum)
	{
	case 2:
		return rand() % 2;
		break;
	case 1:
		return 0;
		break;
	default:
		break;
	}
}

void BattleScene::DealDamage(Entity* attacker, Entity* deffender)
{
	if (deffender->defending == true)
	{
		if ((attacker->atk - deffender->def) < 0) deffender->currentHp = deffender->currentHp;
		else deffender->currentHp = deffender->currentHp - (attacker->atk - deffender->def);
	}
	else deffender->currentHp = deffender->currentHp - attacker->atk;

	deffender->defending = false;
}

void BattleScene::ResumeCombat()
{
	onTurn = false;
	app->sceneManager->atkMenu = false;
	app->sceneManager->defMenu = false;
	tmp = app->entityManager->entityList.start;
	state = WAITING;
	turn = UNKNOWN;
	turnEntity->turnBar.h = 0;
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
				app->render->DrawText(app->render->font, num, 1120, (int)(530 * positionEnemy), 36, 3, { 255, 255, 255, 255 });
				app->render->DrawText(app->render->font, hp, 1150, (int)(555 * positionEnemy), 36, 3, { 255, 255, 255, 255 });
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
				app->render->DrawText(app->render->font, num, 900, (int)(530 * positionAlly), 36, 3, { 255, 255, 255, 255 });
				app->render->DrawText(app->render->font, hp, 930, (int)(555 * positionAlly), 36, 3, { 255, 255, 255, 255 });
				positionAlly += 0.1f;
			}

		}
		tmp = tmp->next;
	}

}