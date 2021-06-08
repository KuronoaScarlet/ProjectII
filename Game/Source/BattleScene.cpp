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
#include "HUD.h"
#include "ParticlesEngine.h"

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
	victory = false;
	lose = false;
	turn = UNKNOWN;
	state = WAITING;
	app->sceneManager->atkMenu = false;
	app->sceneManager->defMenu = false;
	app->sceneManager->combMenu = false; 
	app->sceneManager->enemySelection = 0;
	app->sceneManager->enemySelected = false;
	allySelection = 0;
	itemSelected = false;
	boosted = false;

	app->hud->selectedId = 101;

	hit = app->audio->LoadFx("Assets/Audio/Fx/take_damage.wav");
	powerUp = app->audio->LoadFx("Assets/Audio/Fx/power_up.wav");

	skipBarMax = { 0, 720, 100, 16 };
	skipBar = { 0, 721, 0, 14 };

	//Paso 1: Cargar texturas, botones, música y Fx.
	app->audio->PlayMusic("Assets/Audio/Music/battleSong.ogg");
	screen = app->tex->Load("Assets/Textures/Screens/battle_scene.png");
	combatBox = app->tex->Load("Assets/Textures/combat_box.png");

	//Caritas
	playerFace = app->tex->Load("Assets/Textures/player_face.png");
	allyFace = app->tex->Load("Assets/Textures/ally_face.png");
	dmgEnemyFace = app->tex->Load("Assets/Textures/damage_enemy_face.png");
	tankEnemyFace = app->tex->Load("Assets/Textures/tank_enemy_face.png");
	blncEnemyFace = app->tex->Load("Assets/Textures/balanced_enemy_face.png");


	//BUTTONS---------------------------------------------------------
	attack = new GuiButton(101, { 280, 590, 216, 43 }, "attack");
	attack->SetObserver(this);
	attack->SetTexture(app->tex->Load("Assets/Textures/finger3.png"), app->tex->Load("Assets/Textures/finger4.png"), app->tex->Load("Assets/Textures/finger5.png"));

	defend = new GuiButton(102, { 530, 590, 216, 43 }, "defend");
	defend->SetObserver(this);
	defend->SetTexture(app->tex->Load("Assets/Textures/defend3.png"), app->tex->Load("Assets/Textures/defend4.png"), app->tex->Load("Assets/Textures/defend5.png"));

	combine = new GuiButton(103, { 280, 660, 216, 43 }, "combine");
	combine->SetObserver(this);
	combine->SetTexture(app->tex->Load("Assets/Textures/combine3.png"), app->tex->Load("Assets/Textures/combine4.png"), app->tex->Load("Assets/Textures/combine5.png"));

	run = new GuiButton(104, { 530, 660, 216, 43 }, "run");
	run->SetObserver(this);
	run->SetTexture(app->tex->Load("Assets/Textures/run3.png"), app->tex->Load("Assets/Textures/run4.png"), app->tex->Load("Assets/Textures/run5.png"));

	//--------------------------------------------------------------

	app->render->camera = { 0, 0 };


	//Paso 2: Añadir entidades enemigas. Random (2 o 3).
	rngEnemyNum = 1;
	rngTypeEnemy = rand() % 2;
	int tankID = 600;

	switch (rngEnemyNum)
	{
	case 0:
		app->entityManager->AddEntity({ 880.0f, 288.0f }, Entity::Type::TANK_ENEMY);
		app->entityManager->AddEntity({ 960.0f, 328.0f }, Entity::Type::DAMAGE_ENEMY);
		app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::EQUILIBRATED_ENEMY);

		tankEnemy = new GuiButton(tankID, { 280, 580, 50, 50 }, "tankEnemy");
		tankEnemy->SetObserver(this);
		tankEnemy->SetTexture(app->tex->Load("Assets/Textures/tank_enemy_face.png"), app->tex->Load("Assets/Textures/tank_enemy_selected.png"), app->tex->Load("Assets/Textures/tank_enemy_pressed.png"));

		balancedEnemy = new GuiButton(tankID + 1, { 460, 580, 50, 50 }, "tankEnemy");
		balancedEnemy->SetObserver(this);
		balancedEnemy->SetTexture(app->tex->Load("Assets/Textures/balanced_enemy_face.png"), app->tex->Load("Assets/Textures/balanced_enemy_selected.png"), app->tex->Load("Assets/Textures/balanced_enemy_pressed.png"));

		damageEnemy = new GuiButton(tankID + 2, { 630, 580, 50, 50 }, "tankEnemy");
		damageEnemy->SetObserver(this);
		damageEnemy->SetTexture(app->tex->Load("Assets/Textures/damage_enemy_face.png"), app->tex->Load("Assets/Textures/damage_enemy_selected.png"), app->tex->Load("Assets/Textures/damage_enemy_pressed.png"));

		remainingEnemies = 3;
		break;

	default:
		if (rngTypeEnemy == 0)
		{
			app->entityManager->AddEntity({ 880.0f, 288.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::DAMAGE_ENEMY);

			tankEnemy = new GuiButton(tankID, { 320, 580, 50, 50 }, "tankEnemy");
			tankEnemy->SetObserver(this);
			tankEnemy->SetTexture(app->tex->Load("Assets/Textures/tank_enemy_face.png"), app->tex->Load("Assets/Textures/tank_enemy_selected.png"), app->tex->Load("Assets/Textures/tank_enemy_pressed.png"));

			damageEnemy = new GuiButton(tankID + 1, { 630, 580, 50, 50 }, "tankEnemy");
			damageEnemy->SetObserver(this);
			damageEnemy->SetTexture(app->tex->Load("Assets/Textures/damage_enemy_face.png"), app->tex->Load("Assets/Textures/damage_enemy_selected.png"), app->tex->Load("Assets/Textures/damage_enemy_pressed.png"));

			balancedEnemy = new GuiButton(tankID + 2, { 460, 580, 50, 50 }, "tankEnemy");
			balancedEnemy->SetObserver(this);
			balancedEnemy->SetTexture(app->tex->Load("Assets/Textures/balanced_enemy_face.png"), app->tex->Load("Assets/Textures/balanced_enemy_selected.png"), app->tex->Load("Assets/Textures/balanced_enemy_pressed.png"));

			remainingEnemies = 2;
		}
		else if (rngTypeEnemy == 1)
		{
			app->entityManager->AddEntity({ 880.0f, 288.0f }, Entity::Type::TANK_ENEMY);
			app->entityManager->AddEntity({ 880.0f, 368.0f }, Entity::Type::EQUILIBRATED_ENEMY);

			tankEnemy = new GuiButton(tankID, { 320, 580, 50, 50 }, "tankEnemy");
			tankEnemy->SetObserver(this);
			tankEnemy->SetTexture(app->tex->Load("Assets/Textures/tank_enemy_face.png"), app->tex->Load("Assets/Textures/tank_enemy_selected.png"), app->tex->Load("Assets/Textures/tank_enemy_pressed.png"));
			
			balancedEnemy = new GuiButton(tankID + 1, { 600, 580, 50, 50 }, "tankEnemy");
			balancedEnemy->SetObserver(this);
			balancedEnemy->SetTexture(app->tex->Load("Assets/Textures/balanced_enemy_face.png"), app->tex->Load("Assets/Textures/balanced_enemy_selected.png"), app->tex->Load("Assets/Textures/balanced_enemy_pressed.png"));
			
			damageEnemy = new GuiButton(tankID + 2, { 630, 580, 50, 50 }, "tankEnemy");
			damageEnemy->SetObserver(this);
			damageEnemy->SetTexture(app->tex->Load("Assets/Textures/damage_enemy_face.png"), app->tex->Load("Assets/Textures/damage_enemy_selected.png"), app->tex->Load("Assets/Textures/damage_enemy_pressed.png"));

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
	app->render->DrawText(app->render->font, battleText, 205, 530, 50, 3, { 255, 255, 255, 255 });
	app->render->DrawRectangle({ 188, 534, 1, 194 }, 200, 200, 255, 200);
	PrintFace();
	sprintf_s(turnText, 64, "Turn");
	app->render->DrawText(app->render->font, turnText, 60, 530, 40, 2, { 255, 255, 255, 255 });

	if (turn == PLAYER_TURN && state == SELECT_ACTION && app->sceneManager->atkMenu == false && app->sceneManager->defMenu == false && app->sceneManager->combMenu == false)
	{
		attack->Draw(app->render);
		run->Draw(app->render);
		defend->Draw(app->render);
		combine->Draw(app->render);
	}

	if (turn == PLAYER_TURN && state == SELECT_ACTION && app->sceneManager->atkMenu == true && app->sceneManager->defMenu == false && app->sceneManager->combMenu == false)
	{
		switch (remainingEnemies)
		{
		case 3:
			tankEnemy->Draw(app->render);
			balancedEnemy->Draw(app->render);
			damageEnemy->Draw(app->render);
			break;
		case 2:
			if (app->entityManager->entityList.At(0)->data->type == Entity::Type::TANK_ENEMY && app->entityManager->entityList.At(1)->data->type == Entity::Type::EQUILIBRATED_ENEMY)
			{
				tankEnemy->Draw(app->render);
				balancedEnemy->Draw(app->render);
			}
			if (app->entityManager->entityList.At(0)->data->type == Entity::Type::TANK_ENEMY && app->entityManager->entityList.At(1)->data->type == Entity::Type::DAMAGE_ENEMY)
			{
				tankEnemy->Draw(app->render);
				damageEnemy->Draw(app->render);
			}
			if (app->entityManager->entityList.At(0)->data->type == Entity::Type::EQUILIBRATED_ENEMY && app->entityManager->entityList.At(1)->data->type == Entity::Type::DAMAGE_ENEMY)
			{
				balancedEnemy->Draw(app->render);
				damageEnemy->Draw(app->render);
			}
			break;
		case 1:
			if (app->entityManager->entityList.At(0)->data->type == Entity::Type::TANK_ENEMY)
			{
				tankEnemy->Draw(app->render);
			}
			if (app->entityManager->entityList.At(0)->data->type == Entity::Type::DAMAGE_ENEMY)
			{
				damageEnemy->Draw(app->render);
			}
			if (app->entityManager->entityList.At(0)->data->type == Entity::Type::EQUILIBRATED_ENEMY)
			{
				balancedEnemy->Draw(app->render);
			}
			break;
		default:
			break;

		}
	}

	if (state == LOSE)
	{
		
		app->hud->DrawLooseScreen();
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (app->entityManager->playerData.scene == 1) app->sceneManager->ChangeScene(SCENE1, 0);
			if (app->entityManager->playerData.scene == 2) app->sceneManager->ChangeScene(SCENE12, 0);
		}
	}
	else if (state == VICTORY)
	{
		app->hud->DrawVictoryScreen();

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (app->entityManager->playerData.scene == 1) app->sceneManager->ChangeScene(SCENE1, 0);
			if (app->entityManager->playerData.scene == 2) app->sceneManager->ChangeScene(SCENE12, 0);
			app->sceneManager->CompleteQuest(2);
		}
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
	app->tex->UnLoad(playerFace);
	app->tex->UnLoad(allyFace);
	app->tex->UnLoad(blncEnemyFace);
	app->tex->UnLoad(tankEnemyFace);
	app->tex->UnLoad(dmgEnemyFace);
	attack->CleanUp();
	defend->CleanUp();
	combine->CleanUp();
	run->CleanUp();
	damageEnemy->CleanUp();
	balancedEnemy->CleanUp();
	tankEnemy->CleanUp();
	active = false;

	LOG("Freeing scene");
	return true;
}

void BattleScene::PerformCombat(float dt)
{
	switch (state)
	{
	case WAITING: state = SELECT_ACTION;
		app->hud->selectedId = 101;
		break;

	case SELECT_ACTION:
		if (turn == PLAYER_TURN)
		{
			if (app->sceneManager->atkMenu == false && app->sceneManager->defMenu == false && app->sceneManager->combMenu == false) ShowMenu(dt);

			// ATAQUE
			else if (app->sceneManager->atkMenu == true && app->sceneManager->defMenu == false && app->sceneManager->combMenu == false)
			{
				SelectEnemy(remainingEnemies, dt);
				skipBar.w = 0;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// DEFENSA
			else if (app->sceneManager->atkMenu == false && app->sceneManager->defMenu == true && app->sceneManager->combMenu == false)
			{
				turnEntity->defending = true;
				skipBar.w = 0;
				state = PERFORM_ACTION;
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			// COMBINE
			else if (app->sceneManager->atkMenu == false && app->sceneManager->defMenu == false && app->sceneManager->combMenu == true)
			{
				SelectItem();
				if (itemSelected == true)
				{
					if (app->sceneManager->boost == false)
					{
						app->sceneManager->combMenu = false;
						app->sceneManager->atkMenu = true;
					}
					else
					{
						state = PERFORM_ACTION;
					}
				}
				skipBar.w = 0;
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
			else if (turnEntity->defending == false)
			{
				if (app->sceneManager->boost == false) sprintf_s(battleText, 64, "Atacando! ");
				else
				{
					if (app->sceneManager->itemSelection == 3) sprintf_s(battleText, 64, "Tomando cafe! Soy mas rapido!"); if(boosted == false) Boost(turnEntity);
					if (app->sceneManager->itemSelection == 4) sprintf_s(battleText, 64, "Un Wonster! Mas fuerte y rapido!"); if (boosted == false) Boost(turnEntity);
				}
			}
		}
		else if (turn == ENEMY_TURN)
		{
			sprintf_s(battleText, 64, "El enemigo ataca!");
		}

		//Aquí irán las animaciones en general (Ataques, curas...)

		if (skipBar.w == skipBarMax.w)
		{
			if (turn == PLAYER_TURN && turnEntity->defending == false && app->sceneManager->boost == false) DealDamage(turnEntity, app->entityManager->entityList.At(app->sceneManager->enemySelection)->data);
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
				if (remainingEnemies == 0)
				{
					victory = true;
					if (victory == true)
					{
						app->audio->PlayMusic("Assets/Audio/Music/win_scene_music.ogg");
						app->entityManager->CleanUp();
						state = VICTORY;
					}
				}
				if (remainingAllies == 0)
				{
					lose = true;
					if (lose == true)
					{
						app->audio->PlayMusic("Assets/Audio/Music/lose_screen_music.ogg");
						app->entityManager->CleanUp();
						state = LOSE;
					}
					
				}
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

void BattleScene::SelectEnemy(int enemyNum, float dt)
{
	turnEntity->defending = false;
	sprintf_s(battleText, 64, "Selecciona el enemigo al que atacar!");

	switch (enemyNum)
	{
	case 3:
		tankEnemy->Update(app->input, dt);
		balancedEnemy->Update(app->input, dt);
		damageEnemy->Update(app->input, dt);
		break;
	case 2:
		if (app->entityManager->entityList.At(0)->data->type == Entity::Type::TANK_ENEMY && app->entityManager->entityList.At(1)->data->type == Entity::Type::EQUILIBRATED_ENEMY)
		{
			tankEnemy->Update(app->input, dt);
			balancedEnemy->id = 601;
			balancedEnemy->Update(app->input, dt);
		}
		if (app->entityManager->entityList.At(0)->data->type == Entity::Type::TANK_ENEMY && app->entityManager->entityList.At(1)->data->type == Entity::Type::DAMAGE_ENEMY)
		{
			tankEnemy->Update(app->input, dt);
			damageEnemy->id = 601;
			damageEnemy->Update(app->input, dt);
		}
		if (app->entityManager->entityList.At(0)->data->type == Entity::Type::EQUILIBRATED_ENEMY && app->entityManager->entityList.At(1)->data->type == Entity::Type::DAMAGE_ENEMY)
		{
			damageEnemy->id = 600;
			damageEnemy->Update(app->input, dt);
			balancedEnemy->id = 601;
			balancedEnemy->Update(app->input, dt);
		}
		break;
	case 1:
		if (app->entityManager->entityList.At(0)->data->type == Entity::Type::TANK_ENEMY)
		{
			tankEnemy->Update(app->input, dt);
		}
		if (app->entityManager->entityList.At(0)->data->type == Entity::Type::DAMAGE_ENEMY)
		{
			damageEnemy->id = 600;
			damageEnemy->Update(app->input, dt);
		}
		if (app->entityManager->entityList.At(0)->data->type == Entity::Type::EQUILIBRATED_ENEMY)
		{
			balancedEnemy->id = 600;
			balancedEnemy->Update(app->input, dt);
		}
		break;
	default:
		break;
	}

	if (app->sceneManager->enemySelected == true)
	{
		app->sceneManager->enemySelected = false;
		state = PERFORM_ACTION;
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

void BattleScene::SelectItem()
{
	app->hud->bagEnabled = true;
	if (app->sceneManager->itemSelection > 0)
	{
		itemSelected = true;
		app->hud->bagEnabled = false;
	}
	else
	{
		itemSelected = false;
	}
}

void BattleScene::DealDamage(Entity* attacker, Entity* deffender)
{
	if (itemSelected == true)
	{
		switch (app->sceneManager->itemSelection)
		{
		// Pencil
		case 1: 
			deffender->currentHp = deffender->currentHp - 30;
			app->audio->PlayFx(4, hit);
			app->particleSystem->AddEmitter(EmitterType::SMOKE, deffender->position.x, deffender->position.y);
			break;
		case 2:
			deffender->currentHp = deffender->currentHp - 60;
			app->audio->PlayFx(4, hit);
			app->particleSystem->AddEmitter(EmitterType::SMOKE, deffender->position.x, deffender->position.y);
		}
	}
	else
	{
		if (deffender->defending == true)
		{
			if ((attacker->atk - deffender->def) < 0)
			{
				deffender->currentHp = deffender->currentHp;
				app->audio->PlayFx(4, hit);
				app->particleSystem->AddEmitter(EmitterType::SMOKE, deffender->position.x, deffender->position.y);

			}
			else
			{
				deffender->currentHp = deffender->currentHp - (attacker->atk - deffender->def);
				app->audio->PlayFx(4, hit);
				app->particleSystem->AddEmitter(EmitterType::SMOKE, deffender->position.x, deffender->position.y );
			}
		}
		else
		{
			deffender->currentHp = deffender->currentHp - attacker->atk;
			app->audio->PlayFx(4, hit);
			app->particleSystem->AddEmitter(EmitterType::SMOKE, deffender->position.x, deffender->position.y);
		}
	}
	

	deffender->defending = false;
}

void BattleScene::Boost(Entity* attacker)
{
	if (app->sceneManager->itemSelection == 3)
	{
		attacker->turnTime += 1;
		app->audio->PlayFx(4, powerUp);
		app->particleSystem->AddEmitter(EmitterType::FIRE, attacker->position.x+48, attacker->position.y+96);
	}
	if (app->sceneManager->itemSelection == 4)
	{
		attacker->atk += 30;
		attacker->turnTime += 1;
		app->audio->PlayFx(4, powerUp);
		app->particleSystem->AddEmitter(EmitterType::FIRE, attacker->position.x+48, attacker->position.y+96);
	}
	boosted = true;
}

void BattleScene::ResumeCombat()
{
	onTurn = false;
	app->sceneManager->atkMenu = false;
	app->sceneManager->defMenu = false;
	app->sceneManager->combMenu = false;
	app->sceneManager->boost = false;
	app->sceneManager->enemySelected = false;
	boosted = false;
	tmp = app->entityManager->entityList.start;
	app->sceneManager->itemSelection = 0;
	itemSelected = false;
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

void BattleScene::PrintFace()
{
	if (turn != UNKNOWN)
	{
		switch (turnEntity->type)
		{
		case Entity::Type::PLAYER:
			app->render->DrawTexture(playerFace, 30, 575, NULL);
			break;
		case Entity::Type::ALLY1:
			app->render->DrawTexture(allyFace, 30, 575, NULL);
			break;
		case Entity::Type::EQUILIBRATED_ENEMY:
			app->render->DrawTexture(blncEnemyFace, 30, 575, NULL);
			break;
		case Entity::Type::TANK_ENEMY:
			app->render->DrawTexture(tankEnemyFace, 30, 575, NULL);
			break;
		case Entity::Type::DAMAGE_ENEMY:
			app->render->DrawTexture(dmgEnemyFace, 30, 575, NULL);
			break;
		default:
			break;
		}
	}
}


