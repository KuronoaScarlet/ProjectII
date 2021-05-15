#include "BlueBall.h"
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


BlueBall::BlueBall(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 0,0, 32, 32 });


	currentAnimation = &idleAnimation;
	currentMoodAnimation = &moodAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 46 }), Collider::Type::NPC, listener);
}

bool BlueBall::Start()
{
	return true;
}

bool BlueBall::Update(float dt)
{

	currentAnimation->Update();
	currentMoodAnimation->Update();

	return true;
}

bool BlueBall::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool BlueBall::Interaction()
{
	//app->sceneManager->BlueBall = true;
	if (app->sceneManager->yellowballbutton == true && app->sceneManager->blueballbutton == false && app->sceneManager->redballbutton == false && app->sceneManager->purpleballbutton == false)
	{
		app->sceneManager->blueballbutton = true;
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

void BlueBall::Collision(Collider* coll)
{

}

void BlueBall::CleanUp()
{

}