#include "GateOpen.h"
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


GateOpen::GateOpen(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 837,151, 26, 36 });


	currentAnimation = &idleAnimation;
	currentMoodAnimation = &moodAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 46 }), Collider::Type::NPC, listener);
}

bool GateOpen::Start()
{
	return true;
}

bool GateOpen::Update(float dt)
{

	currentAnimation->Update();
	currentMoodAnimation->Update();
	if (app->sceneManager->yellowballbutton == true && app->sceneManager->blueballbutton == true && app->sceneManager->redballbutton == true && app->sceneManager->purpleballbutton == true)
	{
		pendingToDelete = true;
		collider->pendingToDelete = true;
	}

	return true;
}

bool GateOpen::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool GateOpen::Interaction()
{
	return true;
}

void GateOpen::Collision(Collider* coll)
{

}

void GateOpen::CleanUp()
{

}