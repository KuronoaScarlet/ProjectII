#ifndef _EnemyLantern3_H_
#define _EnemyLantern3_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"

#include "GuiButton.h"

struct SDL_Texture;
struct Collider;

class EnemyLantern3 : Entity
{
public:
	EnemyLantern3(Module* listener, fPoint position, SDL_Texture* texture, Type type);

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
};

#endif // _NPC1_H_
