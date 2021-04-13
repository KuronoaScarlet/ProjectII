#include "Enemy2.h"
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


Enemy2::Enemy2(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 388, 82, 25, 45 });
	idleAnimation.PushBack({ 420, 82, 25, 45 });
	idleAnimation.PushBack({ 452, 82, 25, 45 });
	idleAnimation.PushBack({ 484, 82, 25, 45 });
	idleAnimation.PushBack({ 516, 82, 25, 45 });
	idleAnimation.PushBack({ 548, 82, 25, 45 });
	idleAnimation.speed = 0.2f;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 46 }), Collider::Type::ENEMY, listener);
}

bool Enemy2::Start()
{
	return true;
}

bool Enemy2::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

bool Enemy2::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool Enemy2::Interaction()
{
	return true;
}

void Enemy2::Collision(Collider* coll)
{

}

void Enemy2::CleanUp()
{

}