#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

#define MAX_LISTENERS 300

class Module;

struct Collider
{
	enum Type
	{
		NONE = -1,
		PLAYER,
		FRONTPLAYER,
		WALL,
		TP1TO2,
		TP2TO1,
		NPC,
		ENEMY,
		PENCIL,
		ENEMYLANTERN,
		ENEMYLANTERN2,
		GOLEFT,
		GORIGHT,
		GOUP,
		GODOWN,
		CRATE,
		DUNGEONCP,

		MAX
	};

	enum Height
	{
		UNDEFINED
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	void AddListener(Module* listener);

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Height height;
	Module* listeners[MAX_LISTENERS] = { nullptr };
};


#endif // !__COLLIDER_H__
