#include "Book.h"
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
#include "DialogSystem.h"
#include "EntityManager.h"
#include "PlayerEntity.h"
#include "ParticlesEngine.h"


Book::Book(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0, 0, 32, 32 });
	idleAnimation.PushBack({ 32, 0, 32, 32 });
	idleAnimation.PushBack({ 0, 32, 32, 32 });
	idleAnimation.PushBack({ 32, 32, 32, 32 });
	idleAnimation.speed = 0.1f;

	currentAnimation = &idleAnimation;
	
	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 32, 32 }), Collider::Type::PENCIL, listener);
}

bool Book::Start()
{
	return true;
}

bool Book::Update(float dt)
{

	currentAnimation->Update();
	
	
	return true;
}

bool Book::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool Book::Interaction()
{
	
		app->entityManager->playerData.book++;
		
		picked = true;
		app->particleSystem->AddEmitter(EmitterType::BOOK, position.x, position.y, 120);
		CleanUp();
	
	
	return true;
}

void Book::Collision(Collider* coll)
{
	if ((app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) && picked == false)
	{
		app->entityManager->playerData.book++;
		app->sceneManager->ChangeScene(WIN, 1);
		app->sceneManager->CompleteQuest(3);
		app->playerPosition = { 672.0f, 160.0f };
		app->particleSystem->AddEmitter(EmitterType::BOOK, position.x, position.y, 120);
		CleanUp();
		printf("%d", app->entityManager->playerData.book);
		picked = true;
	}
}

void Book::CleanUp()
{
	pendingToDelete = true;
	collider->pendingToDelete = true;
}