#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Module.h"
#include "Animation.h"
#include "Textures.h"
#include "Point.h"
#include "Timer.h"


class Entity
{
public:
	enum class Type
	{
		UNKNOWN,
		PLAYER,
		NPC1,
		NPC2,
		NPC3,
		NPCQ1,
		NPCQ2,
		NPCQ3,
		ALLY1,
		EQUILIBRATED_ENEMY,
		TANK_ENEMY,
		DAMAGE_ENEMY,
		PENCIL,
		BALL,
		ERASER,
		BOOK,
		CALCULATOR,
		CAN,
		COFFEE,
		FAK,
		LIQUIDPAPER,
		PENCILSHARPENER,
		RUBBER,
		RULE,
		SCOTCHTAPE,
		SNACK,
		ENEMYLANTERN1,
		ENEMYLANTERN2,
		ENEMYLANTERN3,
		CRATE,
		BLUEBALL,
		REDBALL,
		PURPLEBALL,
		YELLOWBALL,
		GATEOPEN
	};

	Entity(Module* listener, fPoint position, SDL_Texture* texture, Type type) : listener(listener), position(position), texture(texture), type(type)
	{}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual bool Interaction()
	{
		return true;
	}

	virtual void Collision(Collider* coll)
	{

	}

	virtual void CleanUp()
	{

	}

	virtual bool TurnBarUpdate()
	{
		turnBar.h -= turnTime;

		if (turnBar.h < (turnBarMax.h + 2))
		{
			turnBar.h = (turnBarMax.h + 2);
			return true;
		}

		return false;
	}

	Module* listener;

	Type type;
	fPoint position;
	SDL_Texture* texture;

	Collider* collider = nullptr;

	SDL_Rect turnBarMax = { 0,0,0,0 };
	SDL_Rect turnBar = { 0,0,0,0 };

	bool pendingToDelete = false;
	int hp, hpPerLvl;
	int atk, atkPerLvl;
	int def, defPerLvl;
	float turnTime;
	int currentHp;
	bool defending = false;
};

#endif // __ENTITY_H__