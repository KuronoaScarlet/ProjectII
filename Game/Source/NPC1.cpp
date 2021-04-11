#include "NPC1.h"
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


NPC1::NPC1(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 96, 18, 32, 46 });

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

	
}

bool NPC1::Start()
{
	return true;
}

bool NPC1::Update(float dt)
{
	currentAnimation->Update();
		
	if (app->entityManager->entityList.At(0)->data->position.x > position.x - 30 &&
		app->entityManager->entityList.At(0)->data->position.x < position.x + 60 &&
		app->entityManager->entityList.At(0)->data->position.y > position.y - 46 &&
		app->entityManager->entityList.At(0)->data->position.y < position.y + 92)
	{
		app->entityManager->entityList.At(0)->data->position.x = app->entityManager->entityList.At(0)->data->position.y;
	}

	return true;
}

bool NPC1::Draw()
{
	SDL_Rect playerRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &playerRect);

	return true;
}

void NPC1::Collision(Collider* coll)
{
	
}

void NPC1::CleanUp()
{

}


