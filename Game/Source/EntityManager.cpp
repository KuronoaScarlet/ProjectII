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
#include "Can.h"
#include "LiquidPaper.h"
#include "Calculator.h"
#include "FirstAidKit.h"
#include "Coffee.h"
#include "PencilSharpener.h"
#include "Rubber.h"
#include "Rule.h"
#include "ScotchTape.h"
#include "Snack.h"
#include "Book.h"
#include "BlueBall.h"
#include "RedBall.h"
#include "PurpleBall.h"
#include "YellowBall.h"
#include "GateOpen.h"

#include "EnemyLantern1.h"
#include "EnemyLantern2.h"
#include "EnemyLantern3.h"
#include "Crate.h"


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
	texNPCQ1 = app->tex->Load("Assets/Textures/Entities/NPC/Rob_32x32.png");
	texNPCQ2 = app->tex->Load("Assets/Textures/Entities/NPC/Rob_32x32.png");
	texNPCQ3 = app->tex->Load("Assets/Textures/Entities/NPC/Rob_32x32.png");

	texItem = app->tex->Load("Assets/Textures/Items/Shine.png");

	texBlueBall = app->tex->Load("Assets/Textures/Entities/Puzzle/blueball.png");
	texRedBall = app->tex->Load("Assets/Textures/Entities/Puzzle/redball.png");
	texPurpleBall = app->tex->Load("Assets/Textures/Entities/Puzzle/purpleball.png");
	texYellowBall = app->tex->Load("Assets/Textures/Entities/Puzzle/yellowball.png");


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
	for (int i = 0; i < entityList.Count(); ++i)
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
		entityList.start->data->position = newPosition;
		playerData.level = e.attribute("lvl").as_int();

	}
	for (e = node.child("items"); e; e = e.next_sibling("items"))
	{
		playerData.pencil = e.attribute("pencil").as_int();
		playerData.sharper = e.attribute("sharper").as_int();
		playerData.ball = e.attribute("ball").as_int();
		playerData.book = e.attribute("book").as_int();
		playerData.calculator = e.attribute("calculator").as_int();
		playerData.cola = e.attribute("cola").as_int();
		playerData.coffee = e.attribute("coffee").as_int();
		playerData.fak = e.attribute("fak").as_int();
		playerData.tipex = e.attribute("tipex").as_int();
		playerData.eraser = e.attribute("eraser").as_int();
		playerData.rule = e.attribute("rule").as_int();
		playerData.celo = e.attribute("celo").as_int();
		playerData.snack = e.attribute("snack").as_int();
		playerData.pencilSharpened = e.attribute("pencilSharp").as_int();
		playerData.wonster = e.attribute("wonster").as_int();
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
			
			eNode = node.append_child("items");
			pugi::xml_attribute pencil = eNode.append_attribute("pencil");
			pencil.set_value(playerData.pencil);
			pugi::xml_attribute sharper = eNode.append_attribute("sharper");
			sharper.set_value(playerData.sharper);
			pugi::xml_attribute ball = eNode.append_attribute("ball");
			ball.set_value(playerData.ball);
			pugi::xml_attribute book = eNode.append_attribute("book");
			book.set_value(playerData.book);
			pugi::xml_attribute calculator = eNode.append_attribute("calculator");
			calculator.set_value(playerData.calculator);
			pugi::xml_attribute cola = eNode.append_attribute("cola");
			cola.set_value(playerData.cola);
			pugi::xml_attribute coffee = eNode.append_attribute("coffee");
			coffee.set_value(playerData.coffee);
			pugi::xml_attribute fak = eNode.append_attribute("fak");
			fak.set_value(playerData.fak);
			pugi::xml_attribute tipex = eNode.append_attribute("tipex");
			tipex.set_value(playerData.tipex);
			pugi::xml_attribute eraser = eNode.append_attribute("eraser");
			eraser.set_value(playerData.eraser);
			pugi::xml_attribute rule = eNode.append_attribute("rule");
			rule.set_value(playerData.rule);
			pugi::xml_attribute celo = eNode.append_attribute("celo");
			celo.set_value(playerData.celo);
			pugi::xml_attribute snack = eNode.append_attribute("snack");
			snack.set_value(playerData.snack);
			pugi::xml_attribute pencilSharp = eNode.append_attribute("pencilSharp");
			pencilSharp.set_value(playerData.pencilSharpened);
			pugi::xml_attribute wonster = eNode.append_attribute("wonster");
			wonster.set_value(playerData.wonster);
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
	case Entity::Type::NPCQ1:
		entityNPCQ1 = (Entity*)(new NPC3((Module*)this, position, texNPCQ1, type));
		entityList.Add(entityNPCQ1);
		break;
	case Entity::Type::NPCQ2:
		entityNPCQ2 = (Entity*)(new NPC3((Module*)this, position, texNPCQ2, type));
		entityList.Add(entityNPCQ2);
		break;
	case Entity::Type::NPCQ3:
		entityNPCQ3 = (Entity*)(new NPC3((Module*)this, position, texNPCQ3, type));
		entityList.Add(entityNPCQ3);
		break;
	case Entity::Type::CRATE:
		entityCrate = (Entity*)(new Crate((Module*)this, position, texPlayer, type));
		entityList.Add(entityCrate);
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
	case Entity::Type::ENEMYLANTERN3:
		entityEnemyLantern3 = (Entity*)(new EnemyLantern3((Module*)this, position, texEnemyLantern1, type));
		entityList.Add(entityEnemyLantern3);
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
		entityBook = (Entity*)(new Book((Module*)this, position, texItem, type));
		entityList.Add(entityBook);
		break;
	case Entity::Type::CALCULATOR:
		entityCalculator = (Entity*)(new Calculator((Module*)this, position, texItem, type));
		entityList.Add(entityCalculator);
		break;
	case Entity::Type::CAN:
		entityCan = (Entity*)(new Can((Module*)this, position, texItem, type));
		entityList.Add(entityCan);
		break;
	case Entity::Type::COFFEE:
		entityCoffee = (Entity*)(new Coffee((Module*)this, position, texItem, type));
		entityList.Add(entityCoffee);
		break;
	case Entity::Type::FAK:
		entityFistAidKit = (Entity*)(new FirstAidKit((Module*)this, position, texItem, type));
		entityList.Add(entityFistAidKit);
		break;
	case Entity::Type::LIQUIDPAPER:
		entityLiquidPaper = (Entity*)(new LiquidPaper((Module*)this, position, texItem, type));
		entityList.Add(entityLiquidPaper);
		break;
	case Entity::Type::PENCILSHARPENER:
		entityPencilSharpener = (Entity*)(new PencilSharpener((Module*)this, position, texItem, type));
		entityList.Add(entityPencilSharpener);
		break;
	case Entity::Type::RUBBER:
		entityRubber = (Entity*)(new Rubber((Module*)this, position, texItem, type));
		entityList.Add(entityRubber);
		break;
	case Entity::Type::RULE:
		entityRule = (Entity*)(new Rule((Module*)this, position, texItem, type));
		entityList.Add(entityRule);
		break;
	case Entity::Type::SCOTCHTAPE:
		entityScotchTape = (Entity*)(new ScotchTape((Module*)this, position, texItem, type));
		entityList.Add(entityScotchTape);
		break;
	case Entity::Type::SNACK:
		entitySnack = (Entity*)(new Snack((Module*)this, position, texItem, type));
		entityList.Add(entitySnack);
		break;
	////////
	///PUZZLE/////
	case Entity::Type::BLUEBALL:
		entityBlueBall = (Entity*)(new BlueBall((Module*)this, position, texBlueBall, type));
		entityList.Add(entityBlueBall);
		break;
	case Entity::Type::REDBALL:
		entityRedBall = (Entity*)(new RedBall((Module*)this, position, texRedBall, type));
		entityList.Add(entityRedBall);
		break;
	case Entity::Type::PURPLEBALL:
		entityPurpleBall = (Entity*)(new PurpleBall((Module*)this, position, texPurpleBall, type));
		entityList.Add(entityPurpleBall);
		break;
	case Entity::Type::YELLOWBALL:
		entityYellowBall = (Entity*)(new YellowBall((Module*)this, position, texYellowBall, type));
		entityList.Add(entityYellowBall);
		break;
	case Entity::Type::GATEOPEN:
		entityGateOpen = (Entity*)(new GateOpen((Module*)this, position, texPlayer, type));
		entityList.Add(entityGateOpen);
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