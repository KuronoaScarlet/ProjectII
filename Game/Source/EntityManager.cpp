#include "EntityManager.h"

#include "App.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"

#include "Scene1.h"
#include "Title.h"

#include "PlayerEntity.h"
#include"NPC1.h"
#include"NPC2.h"
#include"NPC3.h"

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
	texCheckpoint = NULL;
	texPlayer = app->tex->Load("Assets/Textures/player.png");
	texNPC1 = app->tex->Load("Assets/Textures/player.png");
	texNPC2 = app->tex->Load("Assets/Textures/player.png");
	texNPC3 = app->tex->Load("Assets/Textures/player.png");

	return true;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
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
		playerData.resumeButton->Update(app->input, dt);
		playerData.settingsButton->Update(app->input, dt);
		playerData.backToTitleButton->Update(app->input, dt);
		playerData.exitButton->Update(app->input, dt);
	}
	if (app->title->exi)	return false;

	return true;
}

bool EntityManager::PostUpdate()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->Draw();
	}

	playerData.hit = false;

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
		playerData.lives = lives;
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
			lives.set_value(playerData.lives);
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
	case Entity::Type::PLAYER:
		playerEntity = (Entity*)(new PlayerEntity((Module*)this, position, texPlayer, type));
		entityList.Add(playerEntity);
		break;

	case Entity::Type::NPC_1:
		NPC1Entity = (Entity*)(new NPC1((Module*)this, position, texNPC1, type));
		entityList.Add(NPC1Entity);
		break;
	case Entity::Type::NPC_2:
		NPC2Entity = (Entity*)(new NPC2((Module*)this, position, texNPC1, type));
		entityList.Add(NPC2Entity);
		break;
	case Entity::Type::NPC_3:
		NPC3Entity = (Entity*)(new NPC3((Module*)this, position, texNPC1, type));
		entityList.Add(NPC3Entity);
		break;
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