#include "SceneManager.h"
#include "App.h"

#include "Module.h"

#include "Logo.h"
#include "Title.h"
#include "Intro.h"
#include "Scene1.h"
#include "Scene12.h"
#include "SceneBath.h"
#include "SceneGym.h"

SceneManager::SceneManager(SceneType type)
{
	//SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	ChangeScene(type);
}

SceneManager::~SceneManager()
{

}

bool SceneManager::Update(float dt)
{
	if (scene) scene->Update(dt);

	switch (fadeStep)
	{
	case FadeStep::NONE:
		return true;
		break;
	case FadeStep::TO_BLACK:
		alpha += speed * dt;
		if (alpha > 255)
		{
			alpha = 255;
			fadeStep = FadeStep::WAIT;

			if(scene) scene->CleanUp();

			delete scene;
			scene = next_scene;
			next_scene = nullptr;

			scene->active = true;
			//scene->Awake();
			scene->Init();
			scene->Start();
		}
		break;
	case FadeStep::WAIT:
		fadeStep = FadeStep::FROM_BLACK;
		break;
	case FadeStep::FROM_BLACK:
		alpha -= speed * dt;
		if (alpha < 0)
		{
			alpha = 0;
			fadeStep = FadeStep::NONE;
		}
		break;
	}
	return true;
}

bool SceneManager::PostUpdate()
{
	if (scene) scene->PostUpdate();

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, alpha);
	SDL_Rect screen{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	SDL_RenderFillRect(app->render->renderer, &screen);
	return true;
}

void SceneManager::ChangeScene(SceneType type, float new_speed)
{
	if (fadeStep == FadeStep::TO_BLACK) return;

	fadeStep = FadeStep::TO_BLACK;
	speed = new_speed;
	alpha = 0;
	id = type;
	switch (id)
	{
	case LOGO: next_scene = new Logo; break;
	case TITLE: next_scene = new Title; break;
	case INTRO: next_scene = new Intro; break;
	case SCENE1: next_scene = new Scene1; break;
	case SCENE12: next_scene = new Scene12; break;
	case BATH: next_scene = new SceneBath; break;
	case GYM: next_scene = new SceneGym; break;
	}
}

