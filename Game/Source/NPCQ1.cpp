#include "NPCQ1.h"
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


NPCQ1::NPCQ1(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
{
	mood = app->tex->Load("Assets/Textures/Moods/Mood_changer_squeeze_32x32.png");

	idleAnimation.loop = true;
	idleAnimation.PushBack({ 576, 80, 32, 48 });
	idleAnimation.PushBack({ 608, 80, 32, 48 });
	idleAnimation.PushBack({ 640, 80, 32, 48 });
	idleAnimation.PushBack({ 672, 80, 32, 48 });
	idleAnimation.PushBack({ 704, 80, 32, 48 });
	idleAnimation.PushBack({ 736, 80, 32, 48 });
	idleAnimation.speed = 0.2f;

	moodAnimation.PushBack({ 3,20, 24, 31 });
	moodAnimation.PushBack({ 36,20, 24, 31 });
	moodAnimation.PushBack({ 67,20, 24, 31 });
	moodAnimation.loop = false;
	moodAnimation.speed = 0.2f;

	currentAnimation = &idleAnimation;
	currentMoodAnimation = &moodAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 46 }), Collider::Type::NPC, listener);
}

bool NPCQ1::Start()
{
	return true;
}

bool NPCQ1::Update(float dt)
{

	currentAnimation->Update();
	currentMoodAnimation->Update();

	return true;
}

bool NPCQ1::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool NPCQ1::Interaction()
{
	SDL_Rect moodRect = currentMoodAnimation->GetCurrentFrame();
	app->render->DrawTexture(mood, position.x + 25, position.y - 12, &moodRect);

	return true;
}

void NPCQ1::Collision(Collider* coll)
{

}

void NPCQ1::CleanUp()
{

}