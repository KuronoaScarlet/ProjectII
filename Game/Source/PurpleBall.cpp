#include "PurpleBall.h"
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


PurpleBall::PurpleBall(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0,0, 32, 32 });


	currentAnimation = &idleAnimation;
	currentMoodAnimation = &moodAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 46 }), Collider::Type::NPC, listener);
}

bool PurpleBall::Start()
{
	return true;
}

bool PurpleBall::Update(float dt)
{

	currentAnimation->Update();
	currentMoodAnimation->Update();

	return true;
}

bool PurpleBall::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool PurpleBall::Interaction()
{
	//app->sceneManager->PurpleBall = true;
	if (app->sceneManager->yellowballbutton == true && app->sceneManager->blueballbutton == true && app->sceneManager->redballbutton == true && app->sceneManager->purpleballbutton == false)
	{
		app->sceneManager->purpleballbutton = true;
	}
	else
	{
		app->sceneManager->blueballbutton = false;
		app->sceneManager->yellowballbutton = false;
		app->sceneManager->purpleballbutton = false;
		app->sceneManager->redballbutton = false;
	}
	return true;
}

void PurpleBall::Collision(Collider* coll)
{

}

void PurpleBall::CleanUp()
{

}