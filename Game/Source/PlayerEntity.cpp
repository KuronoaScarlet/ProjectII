#include "PlayerEntity.h"
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


PlayerEntity::PlayerEntity(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
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

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 12, 11 }), Collider::Type::PLAYER, listener);
	
}

bool PlayerEntity::Start()
{
	return true;
}

bool PlayerEntity::Update(float dt)
{
	if (!app->entityManager->playerData.pauseCondition)
	{
		//PlayerData Info Containers
		app->entityManager->playerData.position.x = position.x;
		app->entityManager->playerData.position.y = position.y;
	
		//Camera Update
		/*if (position.y <= 230 && position.y >= 20)
		{
			app->render->camera.y = -position.y + 50;
		}*/

		//Player Movement
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE) 
		{
			currentAnimation = &idleAnimation;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= 120 * dt;
			if (currentAnimation != &walkAnimationLeft) 
			{
				walkAnimationLeft.Reset();
				currentAnimation = &walkAnimationLeft;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += 120 * dt;
			if (currentAnimation != &walkAnimationRight) 
			{
				walkAnimationRight.Reset();
				currentAnimation = &walkAnimationRight;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			position.y -= 120 * dt;
			app->render->camera.y += 180 * dt;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			position.y += 120 * dt;
			app->render->camera.y -= 180 * dt;
		}
				
		cameraControl = true;
		currentAnimation->Update();

	}
	

	return true;
}

bool PlayerEntity::Draw()
{
	SDL_Rect playerRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &playerRect);

	return true;
}

void PlayerEntity::Collision(Collider* coll)
{
	
}

void PlayerEntity::CleanUp()
{

}


