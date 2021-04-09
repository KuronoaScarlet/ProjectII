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

private:

	Animation idleAnimation;
	Animation walkAnimationRight;
	Animation walkAnimationLeft;

	Animation* currentAnimation;
		
	float vely = 0.0f;
	float velx = 0.0f;
	float gravity = 0.2f;

	bool godMode = false;
	bool debug;
	bool doubleJump = false;
	bool cameraControl = false;
	bool winCondition = false;
	bool deathCondition = false;

	int scoreFont = -1;
	char scoreText[10] = { "\0" };
};

#endif // _GROUNDENEMY_H_