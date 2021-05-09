#include "EntityManager.h"

#include "App.h"
#include "Log.h"
#include "Textures.h"
#include "Render.h"
#include "Fonts.h"

#include "Scene1.h"
#include "Title.h"

#include "PlayerEntity.h"
#include "Ally1.h"
#include "NPC1.h"
#include "NPC2.h"
#include "NPC3.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "Pencil.h"
#include "Ball.h"
#include "EnemyLantern1.h"
#include "EnemyLantern2.h"


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
	pugi::xml_parse_result result = combatStats.load_file("combat_stats.xml");
	if (result == NULL)
	{
		LOG("Could not load saved game xml file. Pugi error: %s", result.description());
	}

	texPlayer = app->tex->Load("Assets/Textures/Entities/Playable/player.png");
	texAlly1 = app->tex->Load("Assets/Textures/Entities/Playable/Ash_idle_anim_32x32.png");

	texNPC1 = app->tex->Load("Assets/Textures/Entities/NPC/Kid_Mitty_32x32.png");
	texNPC2 = app->tex->Load("Assets/Textures/Entities/NPC/Samuel_32x32.png");
	texNPC3 = app->tex->Load("Assets/Textures/Entities/NPC/Rob_32x32.png");

	texItem = app->tex->Load("Assets/Textures/Items/Shine.png");

	texEnemy1 = app->tex->Load("Assets/Textures/Entities/Enemies/Halloween_Kid_1_idle_anim_32x32.png");
	texEnemyLantern1 = app->tex->Load("Assets/Textures/Entities/NPC/npc_samuel_jacko.png");
	texEnemy3 = app->tex->Load("Assets/Textures/Entities/Enemies/Fishmonger_2_idle_anim_32x32.png");

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

	return true;
}

bool EntityManager::PostUpdate()
{
	for (int i = 0; i < entityList.Count(); i++)
	{
		ListItem<Entity*>* entity = entityList.At(i);
		entity->data->Draw();
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
		entityList.end->data->position = newPosition;
		playerData.level = e.attribute("lvl").as_int();
	}

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node entities = data.append_child("entity");
	pugi::xml_node node = entities.append_child("player");

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
			pugi::xml_attribute lvl = eNode.append_attribute("lvl");
			lvl.set_value(playerData.level);
			eNode.next_sibling("playerdata");
		}
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
		LoadStats(entityPlayer);
		break;
	///////////

	//Allies//
	case Entity::Type::ALLY1:
		entityAlly1 = (Entity*)(new Ally1((Module*)this, position, texAlly1, type));
		entityList.Add(entityAlly1);
		LoadStats(entityAlly1);
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
		LoadStats(entityGhostEnemy);
		break;
	case Entity::Type::TANK_ENEMY:
		entityTankEnemy = (Entity*)(new Enemy2((Module*)this, position, texNPC2, type));
		entityList.Add(entityTankEnemy);
		LoadStats(entityTankEnemy);
		break;
	case Entity::Type::DAMAGE_ENEMY:
		entityCanonEnemy = (Entity*)(new Enemy3((Module*)this, position, texEnemy3, type));
		entityList.Add(entityCanonEnemy);
		LoadStats(entityCanonEnemy);
		break;
	case Entity::Type::ENEMYLANTERN1:
		entityEnemyLantern1 = (Entity*)(new EnemyLantern1((Module*)this, position, texEnemyLantern1, type));
		entityList.Add(entityEnemyLantern1);
		break;
	case Entity::Type::ENEMYLANTERN2:
		entityEnemyLantern2 = (Entity*)(new EnemyLantern2((Module*)this, position, texEnemyLantern1, type));
		entityList.Add(entityEnemyLantern2);
		break;
	//////////
	///ITEMS/////
	case Entity::Type::PENCIL:
		entityPencil = (Entity*)(new Pencil((Module*)this, position, texItem, type));
		entityList.Add(entityPencil);
		break;
	case Entity::Type::BALL:
		entityBall = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityBall);
		break;
	case Entity::Type::BOOK:
		entityBook = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityBook);
		break;
	case Entity::Type::CALCULATOR:
		entityCalculator = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityCalculator);
		break;
	case Entity::Type::CAN:
		entityCan = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityCan);
		break;
	case Entity::Type::COFFEE:
		entityCoffee = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityCoffee);
		break;
	case Entity::Type::FAK:
		entityFistAidKit = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityFistAidKit);
		break;
	case Entity::Type::LIQUIDPAPER:
		entityLiquidPaper = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityLiquidPaper);
		break;
	case Entity::Type::PENCILSHARPENER:
		entityPencilSharpener = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityPencilSharpener);
		break;
	case Entity::Type::RUBBER:
		entityRubber = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityRubber);
		break;
	case Entity::Type::RULE:
		entityRule = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityRule);
		break;
	case Entity::Type::SCOTCHTAPE:
		entityScotchTape = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entityScotchTape);
		break;
	case Entity::Type::SNACK:
		entitySnack = (Entity*)(new Ball((Module*)this, position, texItem, type));
		entityList.Add(entitySnack);
		break;
	////////
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

void EntityManager::LoadStats(Entity* e)
{
	pugi::xml_node node = combatStats.child("combat");

	switch (e->type)
	{
	case Entity::Type::PLAYER:
		node = node.child("player").child("stats");
		break;
	case Entity::Type::ALLY1:
		node = node.child("allies").child("ally1").child("stats");
		break;
	case Entity::Type::TANK_ENEMY:
		node = node.child("enemies").child("tank").child("stats");
		break;
	case Entity::Type::DAMAGE_ENEMY:
		node = node.child("enemies").child("damage").child("stats");
		break;
	case Entity::Type::EQUILIBRATED_ENEMY:
		node = node.child("enemies").child("balanced").child("stats");
		break;
	default:
		break;
	}

	e->atkPerLvl = node.attribute("lvlatk").as_int();
	e->hpPerLvl = node.attribute("lvlhp").as_int();
	e->defPerLvl = node.attribute("lvldef").as_int();

	e->atk = node.attribute("atk").as_int() + (e->atkPerLvl * (playerData.level - 1));
	e->hp = node.attribute("hp").as_int() + (e->hpPerLvl * (playerData.level - 1));
	e->def = node.attribute("def").as_int() + (e->defPerLvl * (playerData.level - 1));

	e->turnTime = node.attribute("turntime").as_float();
	e->currentHp = e->hp;
}