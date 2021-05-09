#include "SceneManager.h"
#include "App.h"
#include "Window.h"

#include "HUD.h"
#include "Map.h"

#include "Logo.h"
#include "Title.h"
#include "Intro.h"
#include "Scene1.h"
#include "Scene12.h"
#include "SceneBath.h"
#include "SceneGym.h"

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{

}

bool SceneManager::Start(SceneType type)
{
    //SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

    ChangeScene(type);

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



    return false;
}

bool SceneManager::Update(float dt)
{
	if (scene) scene->Update(dt);

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

    if (pauseCondition)
    {
        app->render->DrawTexture(pauseMenu, -app->render->camera.x, -app->render->camera.y, NULL);
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



    if (exi == true)
    {
        return false;
    }

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

bool SceneManager::OnGuiMouseClickEvent(GuiControl* control)
{
    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 120)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 130)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 131)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 132)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 133)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 134)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 135)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 136)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 137)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 138)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        if (control->id == 139)
        {
            app->hud->bagEnabled = !app->hud->bagEnabled;
        }
        /*if (control->id == 101)
        {
            if (app->battleScene->playerTurn == true)
            {
                if (control->id == 101)
                {
                    app->battleScene->playerTurn = false;
                    app->battleScene->attackMenu = true;
                }
            }
        }
        if (control->id == 102)
        {
            if (app->battleScene->playerTurn == true)
            {
                if (control->id == 102)
                {
                    app->battleScene->playerTurn = false;
                    app->battleScene->defenseMenu = true;
                    app->battleScene->endTurn = false;
                }
            }
        }*/
        if (control->id == 103)
        {
            app->sceneManager->ChangeScene(SCENE1);
        }
        if (control->id == 13)
        {
            creditsOnScreen = true;
        }
        if (control->id == 18)
        {
            creditsOnScreen = false;
            creditSceneFlag = false;
        }
        if (control->id == 17)
        {
            //Play
            app->sceneManager->settingsEnabled = !app->sceneManager->settingsEnabled;
        }
        if (control->id == 1)
        {
            //LoadGame
            app->loadingGame = true;
            pugi::xml_document savedGame;
            savedGame.load_file("save_game.xml");

            pugi::xml_node generalNode = savedGame.child("save");
            pugi::xml_node map = generalNode.child("map");
            app->map->LoadState(map);

            if (app->currentLevel == 1) ChangeScene(SCENE1);
        }
        else if (control->id == 2)
        {
            //Settings

            configOn = !configOn;
        }
        else if (control->id == 11)
        {
            pauseCondition = false;

            //Back to title
            ChangeScene(TITLE);
        }
        else if (control->id == 4)
        {
            //Exit
            exi = true;
        }
        else if (control->id == 8)
        {
            //Vsync
            if (vsync == true)
            {

                vsync = false;
            }
        }
        else if (control->id == 12)
        {
            ChangeScene(INTRO);
        }
        else if (control->id == 13)
        {
            creditSceneFlag = true;
        }
        else if (control->id == 32)
        {
            pauseCondition = false;
        }
    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 5)
        {
            //Volume
            if (control->bounds.x == 143 || control->bounds.x == 156.5f || control->bounds.x == 170 || control->bounds.x == 183.5f || control->bounds.x == 197 || control->bounds.x == 210.5f || control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
            {
                volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
                app->audio->Volume(volumMusic, '0');
            }
        }
        else if (control->id == 6)
        {
            //FxVolume
            if (control->bounds.x == 143 || control->bounds.x == 156.5f || control->bounds.x == 170 || control->bounds.x == 183.5f || control->bounds.x == 197 || control->bounds.x == 210.5f || control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
            {
                volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
                app->audio->Volume(volumMusic, '1');
            }
        }
    }
    case GuiControlType::CHECKBOX:
    {
        if (control->id == 7)
        {
            //FullScreen
            if (fullSc == false)
            {
                SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
                fullSc = true;
            }
            else
            {
                SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_RESIZABLE);
            }
        }
    }
    default: break;
    }

    return true;
}
