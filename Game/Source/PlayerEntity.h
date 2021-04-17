#ifndef _PLAYERENTITY_H_
#define _PLAYERENTITY_H_

#include "Point.h"
#include "Entity.h"
#include "Animation.h"
#include "DynArray.h"

#include "GuiButton.h"

struct SDL_Texture;
struct Collider;

class PlayerEntity : Entity
{
public:
	PlayerEntity(Module* listener, fPoint position, SDL_Texture* texture, Type type);

	bool Start();

	bool Update(float dt);

	bool Draw();

	void Collision(Collider* coll);

	void CleanUp();

	bool SpeedCounter();
public:


private:
	bool godMode = false;

	Animation idleAnimation;
	Animation walkAnimationRight;
	Animation walkAnimationLeft;
	Animation walkAnimationUp;
	Animation walkAnimationDown;

	Animation battleIdle;

	Animation* currentAnimation;
		
	fPoint tempPlayerPosition;
	
	fPoint lerpCamera;
};

#endif // _GROUNDENEMY_H_