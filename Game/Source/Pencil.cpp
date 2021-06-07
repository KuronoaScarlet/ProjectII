#include "Pencil.h"
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


Pencil::Pencil(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
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

bool Pencil::Start()
{
	return true;
}

bool Pencil::Update(float dt)
{

	currentAnimation->Update();
	
	
	return true;
}

bool Pencil::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool Pencil::Interaction()
{

		app->entityManager->playerData.pencil++;
		app->entityManager->playerData.sharper++;

		CleanUp();
		printf("%d", app->entityManager->playerData.pencil);
		printf("%d", app->entityManager->playerData.sharper);
		picked = true;
		app->particleSystem->AddEmitter(EmitterType::SPARKLES, position.x, position.y, 120);
	
	
	return true;
}

void Pencil::Collision(Collider* coll)
{
	
}

void Pencil::CleanUp()
{
	pendingToDelete = true;
	collider->pendingToDelete = true;
}