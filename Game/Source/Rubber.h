#ifndef _RUBBER_H_
#define _RUBBER_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"

#include "GuiButton.h"

struct SDL_Texture;
struct Collider;

class Rubber : Entity
{
public:
	Rubber(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	bool Interaction();

	void Collision(Collider* coll);

	void CleanUp();

private:

	Animation idleAnimation;
	
	Animation* currentAnimation;

	bool interaction = false;

	bool picked = false;

};

#endif // _NPC1_H_
