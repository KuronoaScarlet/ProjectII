#ifndef _BlueBall_H_
#define _BlueBall_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"

#include "GuiButton.h"

struct SDL_Texture;
struct Collider;

class BlueBall : Entity
{
public:
	BlueBall(Module* listener, fPoint position, SDL_Texture* texture, Type type);

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

#endif // _BlueBall_H_
