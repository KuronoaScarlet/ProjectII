#include "EnemyLantern2.h"
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


EnemyLantern2::EnemyLantern2(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
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

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x + 14, (int)position.y, 32, 45 }), Collider::Type::ENEMYLANTERN2, listener);
	collider2 = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y + 50, 62, 73 }), Collider::Type::ENEMYLANTERN2, listener);
}

bool EnemyLantern2::Start()
{
	return true;
}

bool EnemyLantern2::Update(float dt)
{
	currentAnimation->Update();
	collider->SetPos(position.x+14, position.y);
	collider2->SetPos(position.x, position.y+50);
	if (godown)
	{
		goup = false;
		goleft = false;
		goright = false;
		position.y += 60 * dt;
	}
	else if ( goup == true)
	{
		godown = false;
		goleft = false;
		goright = false;
		position.y += 60 * dt;
	}
	else if (goright == true)
	{
		godown = false;
		goleft = false;
		goup = false;
		position.x += 60 * dt;
	}
	else
	{
	position.x -= 60 * dt;
	}

	return true;
}

bool EnemyLantern2::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool EnemyLantern2::Interaction()
{
	return true;
}

void EnemyLantern2::Collision(Collider* coll)
{
	if ((coll->type == Collider::Type::GODOWN))
	{
		godown = true;
	}
	if ((coll->type == Collider::Type::GOUP))
	{
		goup = true;
	}
	if ((coll->type == Collider::Type::GORIGHT))
	{
		goright = true;
	}
	if ((coll->type == Collider::Type::GOLEFT))
	{
		goleft = true;
	}
}

void EnemyLantern2::CleanUp()
{

}