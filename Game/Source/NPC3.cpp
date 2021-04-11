#include "NPC3.h"
#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene1.h"
#include "Title.h"
#include "Map.h"
#include "Audio.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "Defs.h"


NPC3::NPC3(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 576, 80, 32, 48 });
	idleAnimation.PushBack({ 608, 80, 32, 48 });
	idleAnimation.PushBack({ 640, 80, 32, 48 });
	idleAnimation.PushBack({ 672, 80, 32, 48 });
	idleAnimation.PushBack({ 704, 80, 32, 48 });
	idleAnimation.PushBack({ 736, 80, 32, 48 });
	idleAnimation.speed = 0.2f;

	walkAnimationRight.PushBack({ 0,146, 30, 46 });
	walkAnimationRight.PushBack({ 32,144, 30, 46 });
	walkAnimationRight.PushBack({ 64,146, 30, 46 });
	walkAnimationRight.PushBack({ 96,146, 30, 46 });
	walkAnimationRight.PushBack({ 128,144, 30, 46 });
	walkAnimationRight.PushBack({ 160,146, 30, 46 });

	walkAnimationRight.loop = true;
	walkAnimationRight.speed = 0.2f;

	walkAnimationLeft.PushBack({ 386,146, 30, 46 });
	walkAnimationLeft.PushBack({ 418,144, 30, 46 });
	walkAnimationLeft.PushBack({ 450,146, 30, 46 });
	walkAnimationLeft.PushBack({ 482,146, 30, 46 });
	walkAnimationLeft.PushBack({ 514,144, 30, 46 });
	walkAnimationLeft.PushBack({ 546,146, 30, 46 });
	walkAnimationLeft.loop = true;
	walkAnimationLeft.speed = 0.2f;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 46 }), Collider::Type::NPC, listener);
}

bool NPC3::Start()
{
	return true;
}

bool NPC3::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

bool NPC3::Draw()
{
	SDL_Rect playerRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &playerRect);

	return true;
}

void NPC3::Collision(Collider* coll)
{

}

void NPC3::CleanUp()
{

}


