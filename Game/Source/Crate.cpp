#include "Crate.h"
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


Crate::Crate(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 797,150, 28, 39 });


	currentAnimation = &idleAnimation;
	currentMoodAnimation = &moodAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 46 }), Collider::Type::NPC, listener);
}

bool Crate::Start()
{
	return true;
}

bool Crate::Update(float dt)
{

	currentAnimation->Update();
	currentMoodAnimation->Update();

	return true;
}

bool Crate::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool Crate::Interaction()
{
	app->sceneManager->crate = true;
	pendingToDelete = true;
	collider->pendingToDelete = true;
	return true;
}

void Crate::Collision(Collider* coll)
{

}

void Crate::CleanUp()
{

}