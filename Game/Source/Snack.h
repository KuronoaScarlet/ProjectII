#ifndef _PENCIL_H_
#define _PENCIL_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"

#include "GuiButton.h"

struct SDL_Texture;
struct Collider;

class Pencil : Entity
{
public:
	Pencil(Module* listener, fPoint position, SDL_Texture* texture, Type type);

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