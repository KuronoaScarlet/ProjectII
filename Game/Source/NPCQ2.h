#ifndef __NPCQ2_H__
#define __NPCQ2_H__

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"

#include "GuiButton.h"

struct SDL_Texture;
struct Collider;

class NPCQ2 : Entity
{
public:
	NPCQ2(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	bool Interaction();

	void Collision(Collider* coll);

	void CleanUp();

private:

	Animation idleAnimation;
	Animation moodAnimation;

	Animation* currentAnimation;
	Animation* currentMoodAnimation;

	SDL_Texture* mood;

	bool interaction = false;
};

#endif // _NPC2_H_