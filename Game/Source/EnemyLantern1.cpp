#include "EnemyLantern1.h"
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


EnemyLantern1::EnemyLantern1(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 498, 146, 62, 121 });
	idleAnimation.PushBack({ 561, 148, 62, 121 });
	idleAnimation.PushBack({ 627, 146, 62, 121 });
	idleAnimation.PushBack({ 724, 146, 62, 121 });
	idleAnimation.PushBack({ 819, 148, 62, 121 });
	idleAnimation.PushBack({ 914, 151, 62, 121 });
	idleAnimation.speed = 0.2f;

	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y+50, 62, 73 }), Collider::Type::ENEMYLANTERN, listener);
	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x+14, (int)position.y, 32, 45 }), Collider::Type::ENEMYLANTERN, listener);
}

bool EnemyLantern1::Start()
{
	return true;
}

bool EnemyLantern1::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

bool EnemyLantern1::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool EnemyLantern1::Interaction()
{
	return true;
}

void EnemyLantern1::Collision(Collider* coll)
{

}

void EnemyLantern1::CleanUp()
{

}