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

	leftAnimation.loop = true;
	leftAnimation.PushBack({ 517, 466, 102, 62 });
	leftAnimation.PushBack({ 652, 466, 103, 62 });
	leftAnimation.PushBack({ 793, 467, 103, 62 });
	leftAnimation.PushBack({ 903, 469, 103, 62 });
	leftAnimation.PushBack({ 1030, 467, 102, 62 });
	leftAnimation.PushBack({ 1189, 469, 102, 62 });
	leftAnimation.speed = 0.2f;

	rightAnimation.loop = true;
	rightAnimation.PushBack({ 2, 1, 102, 62 });
	rightAnimation.PushBack({ 181, 0, 102, 62 });
	rightAnimation.PushBack({ 314, 1, 102, 62 });
	rightAnimation.PushBack({ 465, 1, 102, 62 });
	rightAnimation.PushBack({ 571, 0, 102, 62 });
	rightAnimation.PushBack({ 694, 1, 102, 62 });
	rightAnimation.speed = 0.2f;

	upAnimation.loop = true;
	upAnimation.PushBack({ 74, 281, 62, 92 });
	upAnimation.PushBack({ 170, 279, 62, 93 });
	upAnimation.PushBack({ 249, 281, 62, 91 });
	upAnimation.PushBack({ 67, 414, 62, 92 });
	upAnimation.PushBack({ 161, 413, 62, 94 });
	upAnimation.PushBack({ 247, 414, 62, 93 });
	upAnimation.speed = 0.2f;

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
	collider->SetPos(position.x + 14, position.y);
	collider2->SetPos(position.x, position.y + 50);
	
	if (godown == true)
	{		
		currentAnimation = &idleAnimation;
		position.y += 60 * dt;
	}
	else if ( goup == true)
	{
		currentAnimation = &upAnimation;
		position.y -= 60 * dt;
	}
	else if (goright == true)
	{
		currentAnimation = &rightAnimation;
		position.x += 60 * dt;
	}
	else
	{
		currentAnimation = &leftAnimation;
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
		goup = false;
		goleft = false;
		goright = false;
	}
	if ((coll->type == Collider::Type::GOUP))
	{
		godown = false;
		goleft = false;
		goright = false;
		goup = true;
	}
	if ((coll->type == Collider::Type::GORIGHT))
	{
		godown = false;
		goleft = false;
		goup = false;
		goright = true;
	}
	if ((coll->type == Collider::Type::GOLEFT))
	{
		godown = false;
		goright = false;
		goup = false;
		goleft = true;
	}
}

void EnemyLantern2::CleanUp()
{

}