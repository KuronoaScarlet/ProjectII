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

	//collider3 = app->collisions->AddCollider(SDL_Rect({ 10000, 10000, 73, 63 }), Collider::Type::ENEMYLANTERN2, listener);
	//collider4 = app->collisions->AddCollider(SDL_Rect({ 10000, 10000, 62, 73 }), Collider::Type::ENEMYLANTERN2, listener);

}

bool EnemyLantern2::Start()
{
	return true;
}

bool EnemyLantern2::Update(float dt)
{
	currentAnimation->Update();

	
	if (godown == true)
	{	
		collider->SetPos(position.x + 70, position.y);
		collider2->SetPos(position.x+60, position.y +55);
		currentAnimation = &idleAnimation;
		position.y += 120 * dt;		
	}
	else if ( goup == true)
	{
		currentAnimation = &upAnimation;
		position.y -= 120 * dt;
		collider->SetPos(position.x+70, position.y+35);
		collider2->SetPos(position.x+60, position.y-20);
	}
	else if (goright == true)
	{
		currentAnimation = &rightAnimation;
		position.x += 120 * dt;
		collider->SetPos(position.x+60, position.y);
		collider2->SetPos(position.x + 100, position.y);
	}
	else if (goleft == true)
	{
		collider->SetPos(position.x + 74, position.y+30);
		collider2->SetPos(position.x, position.y+30);
		currentAnimation = &leftAnimation;
		position.x -= 120 * dt;
	}
	else
	{
		collider->SetPos(position.x + 74, position.y);
		collider2->SetPos(position.x, position.y);
		currentAnimation = &leftAnimation;
		position.x -= 120 * dt;		
	}

	return true;
}

bool EnemyLantern2::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (godown == true)
	{
		app->render->DrawTexture(texture, position.x+60, position.y, &rect);
	}
	else if (goright == true)
	{
		app->render->DrawTexture(texture, position.x + 60, position.y, &rect);
	}
	else if (goup == true)
	{
		app->render->DrawTexture(texture, position.x + 60, position.y-20, &rect);
	}
	else if (goleft == true)
	{
		app->render->DrawTexture(texture, position.x , position.y + 30, &rect);
	}
	else 
	{
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}

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
		//position.x += 20;
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
		//position.x -= 20;
		//position.y += 40;
		godown = false;
		goright = false;
		goup = false;
		goleft = true;
	}
}

void EnemyLantern2::CleanUp()
{

}