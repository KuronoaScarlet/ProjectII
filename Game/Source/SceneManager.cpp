#include "SceneManager.h"
#include "App.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"

#include "Logo.h"
#include "Title.h"
#include "Intro.h"
#include "Scene1.h"
#include "Scene12.h"
#include "SceneBath.h"
#include "SceneGym.h"

SceneManager::SceneManager()
{
	//SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	//PAUSE.................................................................
	pauseMenu = app->tex->Load("Assets/Textures/Screens/pause_screen.png");
	settingsPost = app->tex->Load("Assets/Textures/postit.png");

	resumeButton = new GuiButton(32, { 517,304, 240, 60 }, "CONTINUE");
	resumeButton->SetObserver(this);
	resumeButton->SetTexture(app->tex->Load("Assets/Textures/resume.png"), app->tex->Load("Assets/Textures/resume_selected.png"), app->tex->Load("Assets/Textures/resume_pressed.png"));

	settingsButton = new GuiButton(17, { 517, 409, 234, 55 }, "SETTINGS");
	settingsButton->SetObserver(this);
	settingsButton->SetTexture(app->tex->Load("Assets/Textures/settings.png"), app->tex->Load("Assets/Textures/settings_selected.png"), app->tex->Load("Assets/Textures/settings_pressed.png"));

	fullScreen = new GuiCheckBox(7, { 620,400, 300, 60 }, "FULLSCREEN");
	fullScreen->SetObserver(this);
	fullScreen->SetTexture(app->tex->Load("Assets/Textures/fs1.png"), app->tex->Load("Assets/Textures/fs2.png"), app->tex->Load("Assets/Textures/fs2.png"));

	exitButton = new GuiButton(11, { 551, 360, 172, 55 }, "CREDITS");
	exitButton->SetObserver(this);
	exitButton->SetTexture(app->tex->Load("Assets/Textures/exit.png"), app->tex->Load("Assets/Textures/exit_selected.png"), app->tex->Load("Assets/Textures/exit_pressed.png"));

	
	//HUD.........................................................

}

SceneManager::~SceneManager()
{

}

bool SceneManager::Update(float dt)
{
	if (scene) scene->Update(dt);
	//----------------------------------------------------------------------------
	
	if (pauseCondition == false)
	{
		settingsEnabled = false;
	}
	if (pauseCondition)
	{
		resumeButton->Update(app->input, dt);
		settingsButton->Update(app->input, dt);
		exitButton->Update(app->input, dt);
		fullScreen->Update(app->input, dt);
		app->audio->Volume(20, '0');

	}
	if (!pauseCondition)
	{
		app->audio->Volume(100, '0');

	}
	resumeButton->bounds.x = -app->render->camera.x + 537;
	resumeButton->bounds.y = -app->render->camera.y + 200;
	settingsButton->bounds.x = -app->render->camera.x + 537;
	settingsButton->bounds.y = -app->render->camera.y + 260;
	exitButton->bounds.x = -app->render->camera.x + 557;
	exitButton->bounds.y = -app->render->camera.y + 360;
	fullScreen->bounds.x = -app->render->camera.x + 900;
	fullScreen->bounds.y = -app->render->camera.y + 200;

	if (pauseCondition)
	{
		app->render->DrawTexture(pauseMenu, -app->render->camera.x, -app->render->camera.y, NULL);
		//fullScreen->Draw(app->render);
		resumeButton->Draw(app->render);
		settingsButton->Draw(app->render);
		exitButton->Draw(app->render);
		if (settingsEnabled)
		{
			app->render->DrawTexture(settingsPost, -app->render->camera.x + 875, -app->render->camera.y + 100, NULL);
			fullScreen->Draw(app->render);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) pauseCondition = !pauseCondition;
	

	//----------------------------------------------------------------------------
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
	/*next_scene->active = true;
	//next_scene->Awake();
	next_scene->Init();
	next_scene->Start();*/
}

