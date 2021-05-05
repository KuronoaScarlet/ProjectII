#include "FadeToBlack.h"

#include "Window.h"
#include "App.h"
#include "Render.h"
#include "Scene1.h"

#include "Defs.h"
#include "Log.h"
#include "Audio.h"

#include "SDL/include/SDL_render.h"

FadeToBlack::FadeToBlack() : Module()
{
	name = "fade";

	//corregir
	screenRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");


	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	switch (currentStep)
	{
	case Fade_Step::NONE:
		return true;
		break;
	case Fade_Step::TO_BLACK:
		alpha += speed * dt;
		if (alpha > 255)
		{
			alpha = 255;
			currentStep = Fade_Step::WAIT;

			if (moduleToDisable->active)
			{
				moduleToDisable->CleanUp();
			}
			if (moduleToEnable->active == false)
			{
				moduleToEnable->Init();
				moduleToEnable->Start();
			}
		}
		break;
	case Fade_Step::WAIT:
		currentStep = Fade_Step::FROM_BLACK;
		break;
	case Fade_Step::FROM_BLACK:
		alpha -= speed * dt;
		if (alpha < 0)
		{
			alpha = 0;
			currentStep = Fade_Step::NONE;
		}
		break;
	}
	printf("%.2f---%.4f---%.2f---%d\n", alpha, dt, speed, currentStep);
	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE)
	{
		return true;
	}

	//float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, alpha);
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::CleanUp()
{
	moduleToDisable = nullptr;
	moduleToEnable = nullptr;

	return true;
}

bool FadeToBlack::Fade(Module* toDisable, Module* toEnable, float new_speed)
{
	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::TO_BLACK)
	{
		return true;
	}
	currentStep = Fade_Step::TO_BLACK;
	speed = new_speed;
	alpha = 0;

	moduleToDisable = toDisable;
	moduleToEnable = toEnable;
	
	return true;
}