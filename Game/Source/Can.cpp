#include "Can.h"
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


Can::Can(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
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

bool Can::Start()
{
	return true;
}

bool Can::Update(float dt)
{

	currentAnimation->Update();
	
	
	return true;
}

bool Can::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool Can::Interaction()
{
	app->entityManager->playerData.cola++;
	CleanUp();
	printf("%d", app->entityManager->playerData.cola);
	picked = true;
	app->particleSystem->AddEmitter(EmitterType::CAN, position.x, position.y, 120);

	
	
	return true;
}

void Can::Collision(Collider* coll)
{
	
}

void Can::CleanUp()
{
	pendingToDelete = true;
	collider->pendingToDelete = true;
}