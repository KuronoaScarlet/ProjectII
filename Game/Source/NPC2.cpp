#include "NPC2.h"
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


NPC2::NPC2(Module* listener, fPoint position, SDL_Texture* texture, Type type) : Entity(listener, position, texture, type)
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

	moodAnimation.PushBack({ 3, 84, 24, 31 });
	moodAnimation.PushBack({ 36, 84, 24, 31 });
	moodAnimation.PushBack({ 67, 84, 24, 31 });
	moodAnimation.loop = false;
	moodAnimation.speed = 0.2f;

	currentAnimation = &idleAnimation;
	currentMoodAnimation = &moodAnimation;

	collider = app->collisions->AddCollider(SDL_Rect({ (int)position.x, (int)position.y, 30, 61 }), Collider::Type::NPC, listener);
}

bool NPC2::Start()
{
	return true;
}

bool NPC2::Update(float dt)
{
	currentAnimation->Update();
	currentMoodAnimation->Update();

	return true;
}

bool NPC2::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	return true;
}

bool NPC2::Interaction()
{
	SDL_Rect moodRect = currentMoodAnimation->GetCurrentFrame();
	app->render->DrawTexture(mood, position.x + 27, position.y - 12, &moodRect);

	return true;
}

void NPC2::Collision(Collider* coll)
{

}

void NPC2::CleanUp()
{

}