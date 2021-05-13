#include "Enemy1.h"
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


Enemy1::Enemy1(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 386, 12, 27, 51 });
	idleAnimation.PushBack({ 418, 12, 27, 51 });
	idleAnimation.PushBack({ 450, 12, 27, 51 });
	idleAnimation.PushBack({ 482, 12, 27, 51 });
	idleAnimation.PushBack({ 514, 12, 27, 51 });
	idleAnimation.PushBack({ 546, 12, 27, 51 });
	idleAnimation.speed = 0.2f;

	currentAnimation = &idleAnimation;

	turnBarMax = { (int)position.x + 35, (int)position.y + 60, 11, -70 };
	turnBar = { (int)position.x + 36, (int)position.y + 59, 9, -2 };

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 27, 51 }), Collider::Type::ENEMY, listener);
}

bool Enemy1::Start()
{
	return true;
}

bool Enemy1::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

bool Enemy1::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	if (app->sceneManager->id == BATTLE)
	{
		app->render->DrawRectangle(turnBarMax, 0, 0, 0, 255);
		app->render->DrawRectangle(turnBar, 255, 185, 0, 255);
	}

	return true;
}

bool Enemy1::Interaction()
{

	return true;
}

void Enemy1::Collision(Collider* coll)
{

}

void Enemy1::CleanUp()
{

}