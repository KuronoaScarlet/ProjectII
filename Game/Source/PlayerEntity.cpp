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
#include "Defs.h"


PlayerEntity::PlayerEntity(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 576, 80, 32, 48 });
	idleAnimation.PushBack({ 608, 80, 32, 48 });
	idleAnimation.PushBack({ 640, 80, 32, 48 });
	idleAnimation.PushBack({ 672, 80, 32, 48 });
	idleAnimation.PushBack({ 704, 80, 32, 48 });
	idleAnimation.PushBack({ 736, 80, 32, 48 });
	idleAnimation.speed = 0.2f;

	walkAnimationRight.PushBack({ 0,146, 30, 46 });
	walkAnimationRight.PushBack({ 32,144, 30, 46 });
	walkAnimationRight.PushBack({ 64,146, 30, 46 });
	walkAnimationRight.PushBack({ 96,146, 30, 46 });
	walkAnimationRight.PushBack({ 128,144, 30, 46 });
	walkAnimationRight.PushBack({ 160,146, 30, 46 });

	walkAnimationRight.loop = true;
	walkAnimationRight.speed = 0.2f;

	walkAnimationLeft.PushBack({ 386,146, 32, 46 });
	walkAnimationLeft.PushBack({ 418,144, 32, 46 });
	walkAnimationLeft.PushBack({ 450,146, 32, 46 });
	walkAnimationLeft.PushBack({ 482,146, 32, 46 });
	walkAnimationLeft.PushBack({ 514,144, 32, 46 });
	walkAnimationLeft.PushBack({ 546,146, 32, 46 });
	walkAnimationLeft.loop = true;
	walkAnimationLeft.speed = 0.2f;

	walkAnimationUp.loop = true;
	walkAnimationUp.speed = 0.2f;
	walkAnimationUp.PushBack({ 192,144, 32, 48 });
	walkAnimationUp.PushBack({ 224,144, 32, 48 });
	walkAnimationUp.PushBack({ 256,144, 32, 48 });
	walkAnimationUp.PushBack({ 288,144, 32, 48 });
	walkAnimationUp.PushBack({ 320,144, 32, 48 });
	walkAnimationUp.PushBack({ 352,144, 32, 48 });

	walkAnimationDown.loop = true;
	walkAnimationDown.speed = 0.2f;
	walkAnimationDown.PushBack({ 576,144, 32, 48 });
	walkAnimationDown.PushBack({ 608,144, 32, 48 });
	walkAnimationDown.PushBack({ 640,144, 32, 48 });
	walkAnimationDown.PushBack({ 672,144, 32, 48 });
	walkAnimationDown.PushBack({ 704,144, 32, 48 });
	walkAnimationDown.PushBack({ 736,144, 32, 48 });


	currentAnimation = &idleAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x + 6, (int)position.y + 34, 22, 12 }), Collider::Type::PLAYER, listener);

	lerpCamera.x = position.x;
	lerpCamera.y = position.y;
}

bool PlayerEntity::Start()
{
	return true;
}

bool PlayerEntity::Update(float dt)
{
	

	tempPlayerPosition = position;
	if (!app->entityManager->playerData.pauseCondition)
	{
		//PlayerData Info Containers
		app->entityManager->playerData.position.x = position.x;
		app->entityManager->playerData.position.y = position.y;

		//Player Movement
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
		{
			currentAnimation = &idleAnimation;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= 150 * dt;
			if (currentAnimation != &walkAnimationLeft && currentAnimation != &walkAnimationUp && currentAnimation != &walkAnimationDown)
			{
				walkAnimationLeft.Reset();
				currentAnimation = &walkAnimationLeft;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += 150 * dt;
			if (currentAnimation != &walkAnimationRight && currentAnimation != &walkAnimationUp && currentAnimation != &walkAnimationDown)
			{
				walkAnimationRight.Reset();
				currentAnimation = &walkAnimationRight;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT )
		{
			position.y -= 150 * dt;
			if (currentAnimation != &walkAnimationUp)
			{
				walkAnimationUp.Reset();
				currentAnimation = &walkAnimationUp;
			}
		}
		
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			position.y += 150 * dt;
			if (currentAnimation != &walkAnimationDown)
			{
				walkAnimationDown.Reset();
				currentAnimation = &walkAnimationDown;
			}
		}
		

		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
		{
			position = { 100.0f, 175.0f };
		}

		cameraControl = true;
		currentAnimation->Update();

		//camera update
		//app->render->DrawRectangle(app->render->camera, 255, 0, 0, 255, false, false);
		lerpCamera.x += (position.x - lerpCamera.x) *dt * 1.0f;
		lerpCamera.y += (position.y - lerpCamera.y) *dt * 1.0f;
		app->render->camera.x = -int(lerpCamera.x) + 640;
		app->render->camera.y = -int(lerpCamera.y) + 360;
	}

	
	collider->SetPos(position.x + 6,position.y + 34);

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
	if(coll->type == Collider::Type::WALL || coll->type == Collider::Type::NPC)
	position = tempPlayerPosition;
}

void PlayerEntity::CleanUp()
{

}


