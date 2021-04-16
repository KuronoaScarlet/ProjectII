#include "EntityManager.h"

#include "App.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"

#include "Scene1.h"
#include "Title.h"

#include "PlayerEntity.h"
#include "NPC1.h"
#include "NPC2.h"
#include "NPC3.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"


EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

bool EntityManager::Awake()
{
	return true;
}

bool EntityManager::Start()
{
	texPlayer = app->tex->Load("Assets/Textures/Entities/Playable/player.png");
	texNPC1 = app->tex->Load("Assets/Textures/Entities/NPC/Kid_Mitty_32x32.png");
	texNPC2 = app->tex->Load("Assets/Textures/Entities/NPC/Samuel_32x32.png");
	texNPC3 = app->tex->Load("Assets/Textures/Entities/NPC/Rob_32x32.png");
	texEnemy1 = app->tex->Load("Assets/Textures/Entities/Enemies/Halloween_Kid_1_idle_anim_32x32.png");
	texEnemy3 = app->tex->Load("Assets/Textures/Entities/Enemies/Fishmonger_2_idle_anim_32x32.png");

	playerData.pauseMenu = app->tex->Load("Assets/Textures/Screens/pause_screen.png");
	//resumeButtton = app->tex->Load("Assets/Textures/resume.png");


	resumeButton = new GuiButton(1, {517,304, 240, 60 }, "CONTINUE");
	resumeButton->SetObserver((Scene1*)this);
	resumeButton->SetTexture(app->tex->Load("Assets/Textures/resume.png"), app->tex->Load("Assets/Textures/resume_selected.png"), app->tex->Load("Assets/Textures/resume_pressed.png"));

	settingsButton = new GuiButton(12, { 517, 370, 234, 55 }, "START");
	settingsButton->SetObserver((Scene1*)this);
	settingsButton->SetTexture(app->tex->Load("Assets/Textures/settings.png"), app->tex->Load("Assets/Textures/settings_selected.png"), app->tex->Load("Assets/Textures/settings_pressed.png"));

	
	backToTitleButton = new GuiButton(2, { 543, 438, 197, 55 }, "OPTIONS");
	backToTitleButton->SetObserver((Scene1*)this);
	backToTitleButton->SetTexture(app->tex->Load("Assets/Textures/settings.png"), app->tex->Load("Assets/Textures/settings_selected.png"), app->tex->Load("Assets/Textures/settings_pressed.png"));

	exitButton = new GuiButton(13, { 551, 514, 172, 55 }, "CREDITS");
	exitButton->SetObserver((Scene1*)this);
	exitButton->SetTexture(app->tex->Load("Assets/Textures/exit.png"), app->tex->Load("Assets/Textures/exit_selected.png"), app->tex->Load("Assets/Textures/exit_pressed.png"));

	return true;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	SDL_Rect polla{ 500 ,500 , 600,600 };
	app->render->DrawRectangle(polla, 242, 176, 98, 255, false, false);
	ListItem<Entity*>* entity = entityList.start;

	while (entity != nullptr)
	{
		if (entity->data->pendingToDelete)
		{
			delete entity->data;
			entityList.Del(entity);
			entity = entity->next;
			continue;
		}

		entity->data->Update(dt);
		entity = entity->next;
	}

	if (app->entityManager->playerData.pauseCondition)
	{
		resumeButton->Update(app->input, dt);
		settingsButton->Update(app->input, dt);
		backToTitleButton->Update(app->input, dt);
		exitButton->Update(app->input, dt);
	}
	if (app->title->exi)	return false;

	return true;
}

bool EntityManager::PostUpdate()
{
	resumeButton->bounds.x = -app->render->camera.x+517;
	resumeButton->bounds.y = -app->render->camera.y+370;
	settingsButton->bounds.x = -app->render->camera.x + 517;
	settingsButton->bounds.y = -app->render->camera.y + 330;
	backToTitleButton->bounds.x = -app->render->camera.x + 543;
	backToTitleButton->bounds.y = -app->render->camera.y + 438;
	exitButton->bounds.x = -app->render->camera.x + 551;
	exitButton->bounds.y = -app->render->camera.y + 514;

	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->Draw();
	}
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && (app->scene1->active == true)) app->entityManager->playerData.pauseCondition = !app->entityManager->playerData.pauseCondition;

	if (app->entityManager->playerData.pauseCondition)
	{
		app->render->DrawTexture(playerData.pauseMenu, -app->render->camera.x, -app->render->camera.y, NULL);

		resumeButton->Draw(app->render);
		app->render->DrawText(app->render->font, "Resume", 530, 120, 60, 5, { 255, 255, 255, 255 });
		settingsButton->Draw(app->render);
		app->render->DrawText(app->render->font, "Settings", 520, 245, 60, 5, { 255, 255, 255, 255 });
		backToTitleButton->Draw(app->render);
		app->render->DrawText(app->render->font, "Back to Title", 465, 370, 60, 5, { 255, 255, 255, 255 });
		exitButton->Draw(app->render);
		app->render->DrawText(app->render->font, "Exit Game", 500, 550, 60, 5, { 255, 255, 255, 255 });
	}
	
	return true;
}

bool EntityManager::CleanUp()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->pendingToDelete = true;
	}

	entityList.Clear();

	return true;
}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	pugi::xml_node entities = data.child("entity");

	pugi::xml_node node = entities.child("player");
	pugi::xml_node node2 = entities.child("enemies");

	pugi::xml_node e;

	int count = 0;

	for (e = node.child("playerdata"); e; e = e.next_sibling("playerdata"))
	{
		float x = e.attribute("x").as_float();
		float y = e.attribute("y").as_float();
		fPoint newPosition = fPoint(x, y);
		Entity* entities = entityList[count];
		if (entities->type == Entity::Type::PLAYER)
		{
			entities->position = newPosition;
			count++;
		}
		int lives = e.attribute("lives").as_int(0);
		int coins = e.attribute("coins").as_int(0);
	}

	count = 1;

	for (e = node2.child("enemy"); e; e = e.next_sibling("enemy"))
	{
		float x = e.attribute("x").as_float();
		float y = e.attribute("y").as_float();
		fPoint newPosition = fPoint(x, y);
		Entity* enemies = entityList[count];
		/*if (enemies->type == Entity::Type::GROUND_ENEMY || enemies->type == Entity::Type::AIR_ENEMY)
		{
			enemies->position = newPosition;
			count++;
		}*/
	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node entities = data.append_child("entity");
	pugi::xml_node node = entities.append_child("player");
	pugi::xml_node node2 = entities.append_child("enemies");

	for (int i = 0; i < entityList.Count(); i++)
	{
		Entity* e = entityList[i];
		if (e->type == Entity::Type::PLAYER)
		{
			pugi::xml_node eNode = node.append_child("playerdata");
			pugi::xml_attribute x = eNode.append_attribute("x");
			x.set_value(e->position.x);
			pugi::xml_attribute y = eNode.append_attribute("y");
			y.set_value(e->position.y);
			pugi::xml_attribute lives = eNode.append_attribute("lives");
			pugi::xml_attribute coins = eNode.append_attribute("coins");
			eNode.next_sibling("playerdata");
		}

		/*if (e->type == Entity::Type::GROUND_ENEMY || e->type == Entity::Type::AIR_ENEMY)
		{
			pugi::xml_node eNode = node2.append_child("enemy");
			pugi::xml_attribute x = eNode.append_attribute("x");
			x.set_value(e->position.x);
			pugi::xml_attribute y = eNode.append_attribute("y");
			y.set_value(e->position.y);
			eNode.next_sibling("enemy");
		}*/
	}

	return true;
}

void EntityManager::AddEntity(fPoint position, Entity::Type type)
{
	switch (type)
	{
	//Player//
	case Entity::Type::PLAYER:
		entityPlayer = (Entity*)(new PlayerEntity((Module*)this, position, texPlayer, type));
		entityList.Add(entityPlayer);
		break;
	///////////

	//NPC's//
	case Entity::Type::NPC1:
		entityNPC1 = (Entity*)(new NPC1((Module*)this, position, texNPC1, type));
		entityList.Add(entityNPC1);
		break;
	case Entity::Type::NPC2:
		entityNPC2 = (Entity*)(new NPC2((Module*)this, position, texNPC2, type));
		entityList.Add(entityNPC2);
		break;
	case Entity::Type::NPC3:
		entityNPC3 = (Entity*)(new NPC3((Module*)this, position, texNPC3, type));
		entityList.Add(entityNPC3);
		break;
	///////////

	//Enemies//
	case Entity::Type::EQUILIBRATED_ENEMY:
		entityGhostEnemy = (Entity*)(new Enemy1((Module*)this, position, texEnemy1, type));
		entityList.Add(entityGhostEnemy);
		break;
	case Entity::Type::TANK_ENEMY:
		entityTankEnemy = (Entity*)(new Enemy2((Module*)this, position, texNPC2, type));
		entityList.Add(entityTankEnemy);
		break;
	case Entity::Type::DAMAGE_ENEMY:
		entityCanonEnemy = (Entity*)(new Enemy3((Module*)this, position, texEnemy3, type));
		entityList.Add(entityCanonEnemy);
		break;
	//////////
	}
}
	



void EntityManager::OnCollision(Collider* a, Collider* b)
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);

		if (entity->data->collider == a && b != nullptr)
		{
			entity->data->Collision(b);
		}

		if (entity->data->collider == b && a != nullptr)
		{
			entity->data->Collision(a);
		}
	}
}