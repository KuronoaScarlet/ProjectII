#include "FirstAidKit.h"
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



FirstAidKit::FirstAidKit(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
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

bool FirstAidKit::Start()
{
	return true;
}

bool FirstAidKit::Update(float dt)
{

	currentAnimation->Update();
	
	
	return true;
}

bool FirstAidKit::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool FirstAidKit::Interaction()
{

		app->entityManager->playerData.fak++;
		CleanUp();
		printf("%d", app->entityManager->playerData.fak);
		picked = true;
		app->particleSystem->AddEmitter(EmitterType::FAK, position.x, position.y, 120);
	
	
	return true;
}

void FirstAidKit::Collision(Collider* coll)
{
	
}

void FirstAidKit::CleanUp()
{
	pendingToDelete = true;
	collider->pendingToDelete = true;
}