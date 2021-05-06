#include "PlayerEntity.h"
#include "App.h"
#include "Render.h"
#include "Collisions.h"
#include "Collider.h"
#include "FadeToBlack.h"
#include "Scene1.h"
#include "BattleScene.h"
#include "Title.h"
#include "Map.h"
#include "Audio.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "Defs.h"
#include "DialogSystem.h"
#include "Input.h"
#include "Scene12.h"

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

	walkAnimationRight.loop = true;
	walkAnimationRight.speed = 0.2f;
	walkAnimationRight.PushBack({ 0,146, 30, 46 });
	walkAnimationRight.PushBack({ 32,144, 30, 46 });
	walkAnimationRight.PushBack({ 64,146, 30, 46 });
	walkAnimationRight.PushBack({ 96,146, 30, 46 });
	walkAnimationRight.PushBack({ 128,144, 30, 46 });
	walkAnimationRight.PushBack({ 160,146, 30, 46 });

	walkAnimationLeft.loop = true;
	walkAnimationLeft.speed = 0.2f;
	walkAnimationLeft.PushBack({ 386,146, 32, 46 });
	walkAnimationLeft.PushBack({ 418,144, 32, 46 });
	walkAnimationLeft.PushBack({ 450,146, 32, 46 });
	walkAnimationLeft.PushBack({ 482,146, 32, 46 });
	walkAnimationLeft.PushBack({ 514,144, 32, 46 });
	walkAnimationLeft.PushBack({ 546,146, 32, 46 });

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

	battleIdle.loop = true;
	battleIdle.speed = 0.2f;
	battleIdle.PushBack({ 0, 80, 29, 47 });
	battleIdle.PushBack({ 32, 80, 29, 47 });
	battleIdle.PushBack({ 64, 80, 29, 47 });
	battleIdle.PushBack({ 96, 80, 29, 47 });
	battleIdle.PushBack({ 128, 80, 29, 47 });
	battleIdle.PushBack({ 160, 80, 29, 47 });

	currentAnimation = &idleAnimation;
	if (app->battleScene->active == true)
	{
		currentAnimation = &battleIdle;
	}

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x + 6, (int)position.y + 34, 22, 12 }), Collider::Type::PLAYER, listener);

	frontCollider = app->collisions->AddCollider(SDL_Rect({ (int)position.x + 6, (int)position.y + 34, 12, 12 }), Collider::Type::FRONTPLAYER, listener);


	lerpCamera.x = position.x;
	lerpCamera.y = position.y;

	app->entityManager->playerData.onDialog = false;
}

bool PlayerEntity::Start()
{
	return true;
}

bool PlayerEntity::Update(float dt)
{
	ListItem<Entity*>* tmp = app->entityManager->entityList.start;
	while (tmp)
	{
		if (tmp->data->type == Type::NPC1)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (app->input->GetKey(SDL_SCANCODE_E ) == KEY_DOWN && app->entityManager->playerData.onDialog == false)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[0]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(0, 7);
					app->dialogueSystem->Id = 0;
				}
				tmp->data->Interaction();
			}
			
		}

		else if (tmp->data->type == Type::NPC2)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->entityManager->playerData.onDialog == false)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[1]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(1, 7);
					app->dialogueSystem->Id = 1;
				}
				tmp->data->Interaction();
			}
		}

		else if (tmp->data->type == Type::NPC3)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->entityManager->playerData.onDialog == false)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[2]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(2, 7);
					app->dialogueSystem->Id = 2;
				}
				tmp->data->Interaction();
			}
			
		}
		else if (tmp->data->type == Type::PENCIL)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				tmp->data->Interaction();
				break;
			}
		}
		else if (tmp->data->type == Type::BALL)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				tmp->data->Interaction();
				break;
			}
		}

		tmp = tmp->next;
	}

	GamePad& pad = app->input->pads[0];
	tempPlayerPosition = position;
	if (!app->entityManager->playerData.pauseCondition)
	{
		if (app->entityManager->playerData.onDialog == false)
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
				if (currentAnimation != &idleAnimation)
				{
					//frontCollider->SetPos(position.x + 8, position.y + 20 + 32);
					idleAnimation.Reset();
					currentAnimation = &idleAnimation;
				}
			}

			float speed = (godMode) ? 300 : 150;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || pad.l_x < 0.0f)
			{
				frontCollider->SetPos(position.x - 20,position.y + 20);
				position.x -= speed * dt;
				if (currentAnimation != &walkAnimationLeft && app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
				{
					walkAnimationLeft.Reset();
					currentAnimation = &walkAnimationLeft;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || pad.l_x > 0.0f)
			{
				frontCollider->SetPos(position.x + 38, position.y + 20);
				position.x += speed * dt;
				if (currentAnimation != &walkAnimationRight && app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
				{
					walkAnimationRight.Reset();
					currentAnimation = &walkAnimationRight;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || pad.l_y < 0.0f)
			{
				frontCollider->SetPos(position.x + 8, position.y - 20);
				position.y -= speed * dt;
				if (currentAnimation != &walkAnimationUp)
				{
					walkAnimationUp.Reset();
					currentAnimation = &walkAnimationUp;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || pad.l_y > 0.0f)
			{
				frontCollider->SetPos(position.x + 8, position.y + 20 + 32);
				position.y += speed * dt;
				if (currentAnimation != &walkAnimationDown)
				{
					walkAnimationDown.Reset();
					currentAnimation = &walkAnimationDown;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			{
				godMode = !godMode;
			}

			if (app->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
			{
				app->input->ShakeController(0, 12, 0.33f);
			}

			//camera update

			lerpCamera.x += (position.x - lerpCamera.x) * dt * 1.0f;
			lerpCamera.y += (position.y - lerpCamera.y) * dt * 1.0f;
			/*if (lerpCamera.x < 640) lerpCamera.x = 640;
			if (lerpCamera.x > 300) lerpCamera.x = 300;
			if (lerpCamera.y < 380) lerpCamera.y = 380;
			if (lerpCamera.y > 1480) lerpCamera.y = 1480;*/
			printf("%.2f---%.2f\n", position.x, position.y);
			app->render->camera.x = -int(lerpCamera.x) + 640;
			app->render->camera.y = -int(lerpCamera.y) + 360;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();

	currentAnimation->Update();
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
	if ((coll->type == Collider::Type::WALL || coll->type == Collider::Type::NPC) && godMode == false)
	{
		position = tempPlayerPosition;
	}
	if ((coll->type == Collider::Type::TP1TO2 && godMode == false))
	{
		Tp(coll);
		app->scene1->passingToLvl2 = true;
	}
	if ((coll->type == Collider::Type::TP2TO1 && godMode == false))
	{
		Tp(coll);
		app->scene12->passingToLvl1 = true;
	}
	if ((coll->type == Collider::Type::ENEMYLANTERN && godMode == false))
	{
		position.x = 320.0f;
		position.y = 768.0f;
	}
	if ((coll->type == Collider::Type::ENEMYLANTERN2 && godMode == false))
	{
		position.x = 320.0f;
		position.y = 768.0f;
	}
}

void PlayerEntity::CleanUp()
{

}
bool PlayerEntity::Tp(Collider* collider)
{
	//SCENE 1 TO SCENE 2
	if ((collider->rect.y == 448 && collider->rect.x == 1024))
	{
		app->playerPosition = { 64.0f, 416.0f };
		return true;
	}
	if (collider->rect.x == 992 && collider->rect.y == 1056 )
	{
		app->playerPosition = { 64.0f, 1024.0f };
		return false;
	}
	//SCENE 2 TO SCENE 1
	if (collider->rect.x == 32 && collider->rect.y == 416)
	{
		app->playerPosition = { 960.0f, 448.0f };
		return true;
	}
	if (collider->rect.x == 32 && collider->rect.y == 1024)
	{
		app->playerPosition = { 960.0f, 1088.0f };
		return false;
	}
}