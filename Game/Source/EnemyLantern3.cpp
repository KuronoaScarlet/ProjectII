#include "EnemyLantern3.h"
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


EnemyLantern3::EnemyLantern3(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	upAnimation.loop = true;
	upAnimation.PushBack({ 74, 281, 62, 92 });
	upAnimation.PushBack({ 170, 279, 62, 93 });
	upAnimation.PushBack({ 249, 281, 62, 91 });
	upAnimation.PushBack({ 67, 414, 62, 92 });
	upAnimation.PushBack({ 161, 413, 62, 94 });
	upAnimation.PushBack({ 247, 414, 62, 93 });
	upAnimation.speed = 0.2f;


	currentAnimation = &upAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 62, 73 }), Collider::Type::ENEMYLANTERN, listener);
	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x+15, (int)position.y+40, 32, 45 }), Collider::Type::ENEMYLANTERN, listener);
}

bool EnemyLantern3::Start()
{
	return true;
}

bool EnemyLantern3::Update(float dt)
{
	currentAnimation->Update();

	return true;
}

bool EnemyLantern3::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool EnemyLantern3::Interaction()
{
	return true;
}

void EnemyLantern3::Collision(Collider* coll)
{

}

void EnemyLantern3::CleanUp()
{

}