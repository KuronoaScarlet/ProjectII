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

	virtual bool SpeedCounter()
	{
		if (combatTimer.ReadCombat() > turnTime)
		{
			combatTimer.counter = 0;
			combatTimer.Start();
			return true;
		}

		return false;
	}

	Module* listener;

	Type type;
	fPoint position;
	SDL_Texture* texture;

	Collider* collider = nullptr;

	bool pendingToDelete = false;

	int hp, hpPerLvl;
	int atk, atkPerLvl;
	int def, defPerLvl;
	float turnTime;
	int currentHp;
	bool defending = false;
	bool dead = false;

	Timer combatTimer;

};

#endif // __ENTITY_H__