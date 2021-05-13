#include "Ally1.h"
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


Ally1::Ally1(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.speed = 0.2f;
	idleAnimation.PushBack({ 2, 16, 27, 47 });
	idleAnimation.PushBack({ 34, 16, 27, 47 });
	idleAnimation.PushBack({ 66, 16, 27, 47 });
	idleAnimation.PushBack({ 98, 16, 27, 47 });
	idleAnimation.PushBack({ 130, 16, 27, 47 });
	idleAnimation.PushBack({ 162, 16, 27, 47 });

	currentAnimation = &idleAnimation;

	turnBarMax = { (int)position.x - 16, (int)position.y + 60, 11, -70 };
	turnBar = { (int)position.x - 15, (int)position.y + 59, 9, -5 };

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 27, 51 }), Collider::Type::PLAYER, listener);
}

bool Ally1::Start()
{
	return true;
}

bool Ally1::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

bool Ally1::Draw()
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

bool Ally1::Interaction()
{
	return true;
}

void Ally1::Collision(Collider* coll)
{

}

void Ally1::CleanUp()
{

}