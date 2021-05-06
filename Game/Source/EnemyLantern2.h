#ifndef _EnemyLantern2_H_
#define _EnemyLantern2_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"

#include "GuiButton.h"

struct SDL_Texture;
struct Collider;

class EnemyLantern2 : Entity
{
public:
	EnemyLantern2(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	bool Interaction();

	void Collision(Collider* coll);

	void CleanUp();

private:

	Animation idleAnimation;
	Animation leftAnimation;
	Animation rightAnimation;
	Animation upAnimation;
	Animation downAnimation;

	Animation* currentAnimation;

	Collider* collider2 = nullptr;

	bool goup = false;
	bool godown = false;
	bool goleft = false;
	bool goright = false;

};

#endif // _NPC1_H_

