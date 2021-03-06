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
#include "SceneManager.h"
#include "HUD.h"
#include "ParticlesEngine.h"

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

	crateAnimation.loop = true;
	crateAnimation.speed = 0.2f;
	crateAnimation.PushBack({ 797,150, 28, 39 });

	currentAnimation = &idleAnimation;
	if (app->sceneManager->id == SceneType::BATTLE)
	{
		currentAnimation = &battleIdle;
	}

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x + 6, (int)position.y + 34, 22, 12 }), Collider::Type::PLAYER, listener);

	frontCollider = app->collisions->AddCollider(SDL_Rect({ (int)position.x + 6, (int)position.y + 34, 12, 12 }), Collider::Type::FRONTPLAYER, listener);

	turnBarMax = { (int)position.x - 16, (int)position.y + 60, 11, -70 };
	turnBar = { (int)position.x - 15, (int)position.y + 59, 9, -20 };

	lerpCamera.x = position.x;
	lerpCamera.y = position.y;

	app->audio->LoadFx("Assets/Audio/Fx/pick_item.wav");

	app->entityManager->playerData.onDialog = false;
}

bool PlayerEntity::Start()
{
	return true;
}

bool PlayerEntity::Update(float dt)
{
	ListItem<Entity*>* tmp = app->entityManager->entityList.start;
	GamePad& pad = app->input->pads[0];
	while (tmp)
	{
		if (tmp->data->type == Type::NPC1)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					ray1 = false;
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
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[1]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(1, 7);
					app->dialogueSystem->Id = 1;
					app->audio->PlayFx(3, pickItem);
				}
				tmp->data->Interaction();
			}
		}

		else if (tmp->data->type == Type::NPC3)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[2]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(2, 7);
					app->dialogueSystem->Id = 2;
					app->audio->PlayFx(3, pickItem);
				}
				tmp->data->Interaction();
			}
			
		}
		else if (tmp->data->type == Type::NPCQ1)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[4]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(4, 7);
					app->dialogueSystem->Id = 4;
					app->audio->PlayFx(3, pickItem);
				}
				tmp->data->Interaction();
			}

		}
		else if (tmp->data->type == Type::NPCQ2)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[5]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(5, 7);
					app->dialogueSystem->Id = 5;
					app->audio->PlayFx(3, pickItem);
				}
				tmp->data->Interaction();
			}

		}
		else if (tmp->data->type == Type::NPCQ3)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E ) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->entityManager->playerData.onDialog = true;
					app->dialogueSystem->actionChecks = 0;
					app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[6]->dialogueNodes[0];
					app->dialogueSystem->PerformDialogue(6, 7);
					app->dialogueSystem->Id = 6;
					app->audio->PlayFx(3, pickItem);
				}
				tmp->data->Interaction();
			}
		}
		else if (tmp->data->type == Type::PENCIL)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->audio->PlayFx(3, pickItem);
					tmp->data->Interaction();
					break;
				}
			}
		}
		else if (tmp->data->type == Type::BALL)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->audio->PlayFx(3, pickItem);
					tmp->data->Interaction();
					break;
				}
			}
		}
		else if (tmp->data->type == Type::CRATE)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					tmp->data->Interaction();
					app->audio->PlayFx(3, pickItem);
					break;
				}
				char crateText[80] = { 0 };
				sprintf_s(crateText, 80, "Presiona la E para esconderte en la caja.");
				app->render->DrawText(app->render->font, crateText, 0, 0, 40, 0, { 255,255, 255, 255 });
			}
		}
		else if (tmp->data->type == Type::BLUEBALL)
		{
			if (position.DistanceTo(tmp->data->position) < 30)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					tmp->data->Interaction();
					app->audio->PlayFx(3, pickItem);
					break;
				}
				char crateText[80] = { 0 };
				sprintf_s(crateText, 80, "Una pelota azul, parece que tiene un boton, pulsa E para activarlo.");
				app->render->DrawText(app->render->font, crateText, 0, 0, 40, 0, { 255,255, 255, 255 });
			}
		}
		else if (tmp->data->type == Type::REDBALL)
		{
			if (position.DistanceTo(tmp->data->position) < 30)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					tmp->data->Interaction();
					app->audio->PlayFx(3, pickItem);
					break;
				}
				char crateText[80] = { 0 };
				sprintf_s(crateText, 80, "Una pelota roja, parece que tiene un boton, pulsa E para activarlo.");
				app->render->DrawText(app->render->font, crateText, 0, 0, 40, 0, { 255,255, 255, 255 });
			}
		}
		else if (tmp->data->type == Type::YELLOWBALL)
		{
			if (position.DistanceTo(tmp->data->position) < 30)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					tmp->data->Interaction();
					app->audio->PlayFx(3, pickItem);
					break;
				}
				char crateText[80] = { 0 };
				sprintf_s(crateText, 80, "Una pelota amarilla, parece que tiene un boton, pulsa E para activarlo.");
				app->render->DrawText(app->render->font, crateText, 0, 0, 40, 0, { 255,255, 255, 255 });
			}
		}
		else if (tmp->data->type == Type::PURPLEBALL)
		{
			if (position.DistanceTo(tmp->data->position) < 30)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					tmp->data->Interaction();
					app->audio->PlayFx(3, pickItem);
					break;
				}
				char crateText[80] = { 0 };
				sprintf_s(crateText, 80, "Una pelota lila, parece que tiene un bot�n, pulsa E para activarlo.");
				app->render->DrawText(app->render->font, crateText, 0, 0, 40, 0, { 255,255, 255, 255 });
			}
		}
		else if (tmp->data->type == Type::CAN)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E ) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->audio->PlayFx(3, pickItem);
					tmp->data->Interaction();
					break;
				}
			}
		}

		else if (tmp->data->type == Type::COFFEE)
		{
			if (position.DistanceTo(tmp->data->position) < 50)
			{
				if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
				{
					app->audio->PlayFx(3, pickItem);
					tmp->data->Interaction();
					break;
				}
			}
		}

		else if (tmp->data->type == Type::RUBBER)
		{
		if (position.DistanceTo(tmp->data->position) < 50)
		{
			if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
			{
				app->audio->PlayFx(3, pickItem);
				tmp->data->Interaction();
				break;
			}
		}
		}
		else if (tmp->data->type == Type::RULE)
		{
		if (position.DistanceTo(tmp->data->position) < 50)
		{
			if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
			{
				app->audio->PlayFx(3, pickItem);
				tmp->data->Interaction();
				break;
			}
		}
		}
		else if (tmp->data->type == Type::SNACK)
		{
		if (position.DistanceTo(tmp->data->position) < 50)
		{
			if (((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x) && app->entityManager->playerData.onDialog == false) && ray1 == true)
			{
				app->audio->PlayFx(3, pickItem);
				tmp->data->Interaction();
				break;
			}
		}
		}

		if (!pad.x && !pad.up)
		{
			ray1 = true;
		}

		tmp = tmp->next;
	}

	
	tempPlayerPosition = position;
	if (!app->hud->pauseCondition && !app->hud->cantMoveInInvetory)
	{
		if (app->entityManager->playerData.onDialog == false && app->sceneManager->id != SceneType::BATTLE)
		{
			//PlayerData Info Containers
			app->entityManager->playerData.position.x = position.x;
			app->entityManager->playerData.position.y = position.y;

			//Player Movement
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
				&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
				&& app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE
				&& app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE
				&& pad.l_x == 0  && pad.l_y == 0)
			{
				if (currentAnimation != &idleAnimation)
				{
					if (app->sceneManager->crate == false)
					{
						//frontCollider->SetPos(position.x + 8, position.y + 20 + 32);
						idleAnimation.Reset();
						currentAnimation = &idleAnimation;
					}
					else
					{
						crateAnimation.Reset();
						currentAnimation = &crateAnimation;
					}
				}
			}

			float speed = (godMode) ? 300 : 150;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || pad.l_x < 0.0f)
			{
				frontCollider->SetPos(position.x - 20,position.y + 20);
				position.x -= speed * dt;
				collider->SetPos(position.x + 6, position.y + 34);
				if (currentAnimation != &walkAnimationLeft && app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT
					&& pad.l_y == 0.0f)
					if (app->sceneManager->crate == false)
					{
						{
							walkAnimationLeft.Reset();
							currentAnimation = &walkAnimationLeft;
						}
					}
					else
					{
						crateAnimation.Reset();
						currentAnimation = &crateAnimation;
					}
			}
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || pad.l_x > 0.0f)
			{
				frontCollider->SetPos(position.x + 38, position.y + 20);
				position.x += speed * dt;
				collider->SetPos(position.x + 6, position.y + 34);
				if (currentAnimation != &walkAnimationRight && app->input->GetKey(SDL_SCANCODE_W) != KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT
					&& pad.l_y == 0.0f)
					if (app->sceneManager->crate == false)
					{
						{
							walkAnimationRight.Reset();
							currentAnimation = &walkAnimationRight;
						}
					}
				else
				{
					crateAnimation.Reset();
					currentAnimation = &crateAnimation;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || pad.l_y < 0.0f)
			{
				frontCollider->SetPos(position.x + 8, position.y - 20);
				position.y -= speed * dt;
				collider->SetPos(position.x + 6, position.y + 34);
				
				if (app->sceneManager->crate == false)
				{
					if (currentAnimation != &walkAnimationUp)
					{
						walkAnimationUp.Reset();
						currentAnimation = &walkAnimationUp;
					}
				}
				else
				{
					crateAnimation.Reset();
					currentAnimation = &crateAnimation;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || pad.l_y > 0.0f)
			{
				frontCollider->SetPos(position.x + 8, position.y + 20 + 32);
				position.y += speed * dt;
				collider->SetPos(position.x + 6, position.y + 34);
				if (currentAnimation != &walkAnimationDown)
					if (app->sceneManager->crate == false)
					{
						{
							walkAnimationDown.Reset();
							currentAnimation = &walkAnimationDown;
						}
					}
				else
				{
					crateAnimation.Reset();
					currentAnimation = &crateAnimation;
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

			//CAMERA MANAGEMENT----------------------------------------

			float diffx = position.x - lerpCamera.x;
			float diffy = position.y - lerpCamera.y;

			//scaled min dist�
			if (abs(diffx) > 15.0f) lerpCamera.x += diffx * dt * 1.0f * scale_zoom;
			if (abs(diffy) > 15.0f) lerpCamera.y += diffy * dt * 1.0f * scale_zoom;

			int xx = -lerpCamera.x + 640.0f / scale_zoom;
			int yy = -lerpCamera.y + 360.0f / scale_zoom;

			app->render->camera.x = xx;
			app->render->camera.y = yy;
			//-------------------------------------------------------
			
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();

	currentAnimation->Update();
	if (app->sceneManager->scenegym == false) 
	{
		collider->SetPos(position.x + 6, position.y + 34);
	}
	
	if(position == tempPlayerPosition && app->sceneManager->scenegym == true && app->sceneManager->crate == true)
	{
		collider->SetPos(-1000, -1000);
	}
	return true;
}

bool PlayerEntity::Draw()
{
	SDL_Rect playerRect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &playerRect);

	if (app->sceneManager->id == BATTLE)
	{
		app->render->DrawRectangle(turnBarMax, 0, 0, 0, 255);
		app->render->DrawRectangle(turnBar, 255, 185, 0, 255);
	}

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
		if (app->entityManager->playerData.tp == false)
		{
			app->entityManager->playerData.tp = true;
			Tp(coll);
			app->sceneManager->ChangeScene(SCENE12, 0);
		}
	}

	if ((coll->type == Collider::Type::TP2TO1 && godMode == false))
	{
		Tp(coll);
		app->sceneManager->ChangeScene(SCENE1, 0);

	}

	if ((coll->type == Collider::Type::TPGYMTOBATH && godMode == false))
	{
		Tp(coll);
		app->sceneManager->ChangeScene(BATH, 0);

	}

	if ((coll->type == Collider::Type::TPBATHTOGYM && godMode == false))
	{
		Tp(coll);
		app->sceneManager->ChangeScene(GYM, 0);

	}

	if ((coll->type == Collider::Type::DUNGEONCP && godMode == false))
	{
		app->sceneManager->checkpointgym = true;
	}
	if ((coll->type == Collider::Type::GETOUTBOX && godMode == false))
	{
		app->sceneManager->crate = false;
	}
	if ((coll->type == Collider::Type::ENEMYLANTERN && godMode == false))
	{
		if (app->sceneManager->scenebath == true)
		{
			position.x = 320.0f;
			position.y = 768.0f;
		}

		if (app->sceneManager->scenegym == true && app->sceneManager->checkpointgym == false)
		{
			position.x = 32.0f;
			position.y = 96.0f;
		}

		if (app->sceneManager->scenegym == true && app->sceneManager->checkpointgym == true)
		{
			position.x = 608.0f;
			position.y = 620.0f;
		}
	}
	if ((coll->type == Collider::Type::ENEMYLANTERN2 && godMode == false))
	{
		if (app->sceneManager->scenebath == true)
		{
			position.x = 320.0f;
			position.y = 768.0f;
		}

		if (app->sceneManager->scenegym == true && app->sceneManager->checkpointgym == false)
		{
			position.x = 32.0f;
			position.y = 96.0f;
		}

		if (app->sceneManager->scenegym == true && app->sceneManager->checkpointgym == true)
		{
			position.x = 608.0f;
			position.y = 620.0f;
		}
	}
}

void PlayerEntity::CleanUp()
{

}
bool PlayerEntity::Tp(Collider* collider)
{
	//SCENE 1 TO SCENE 2
	if ((collider->rect.x == 1024 && collider->rect.y == 416))
	{
		app->playerPosition = { 96.0f, 416.0f };
		
		return true;
	}
	if (collider->rect.x == 992 && collider->rect.y == 1056 )
	{
		app->playerPosition = { 96.0f, 1024.0f };
		
		return false;
	}
	//SCENE 2 TO SCENE 1
	if (collider->rect.x == 64 && collider->rect.y == 416)
	{
		app->playerPosition = { 860.0f, 448.0f };
		
		return true;
	}
	if (collider->rect.x == 64 && collider->rect.y == 1024)
	{
		app->playerPosition = { 860.0f, 1088.0f };
		
		return false;
	}
}