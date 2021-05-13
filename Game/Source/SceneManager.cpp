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
#include "BattleScene.h"

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

    musicSlider = new GuiSlider(100, { 900,300, 60, 60 }, "FULLSCREEN");
    musicSlider->SetObserver(this);
    musicSlider->SetTexture(app->tex->Load("Assets/Textures/fx.png"), app->tex->Load("Assets/Textures/fx_selected.png"), app->tex->Load("Assets/Textures/fx_focused.png"));

    fxSlider = new GuiSlider(100, { 900,350, 60, 60 }, "FULLSCREEN");
    fxSlider->SetObserver(this);
    fxSlider->SetTexture(app->tex->Load("Assets/Textures/fx.png"), app->tex->Load("Assets/Textures/fx_selected.png"), app->tex->Load("Assets/Textures/fx_focused.png"));

    exitButton = new GuiButton(11, { 551, 360, 172, 55 }, "CREDITS");
    exitButton->SetObserver(this);
    exitButton->SetTexture(app->tex->Load("Assets/Textures/exit.png"), app->tex->Load("Assets/Textures/exit_selected.png"), app->tex->Load("Assets/Textures/exit_pressed.png"));

    musicSliderBack = { 900,300,300,40 };
    fxSliderBack = { 900,350,300,40 };

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
        if (settingsEnabled)
        {
            musicSlider->Update(app->input, dt);
            fxSlider->Update(app->input, dt);
        }
      //  app->audio->Volume(20, '0');

    }
    if (!pauseCondition)
    {
       
       // app->audio->Volume(100, '0');

    }
    resumeButton->bounds.x = -app->render->camera.x + 537;
    resumeButton->bounds.y = -app->render->camera.y + 200;
    settingsButton->bounds.x = -app->render->camera.x + 537;
    settingsButton->bounds.y = -app->render->camera.y + 260;
    exitButton->bounds.x = -app->render->camera.x + 557;
    exitButton->bounds.y = -app->render->camera.y + 360;

    fullScreen->bounds.x = -app->render->camera.x + 900;
    fullScreen->bounds.y = -app->render->camera.y + 200;

   // musicSlider->bounds.x = -app->render->camera.x + 900;
    musicSlider->bounds.y = -app->render->camera.y + 300;

    //fxSlider->bounds.x = -app->render->camera.x + 900;
    fxSlider->bounds.y = -app->render->camera.y + 350;

    //RECT OF THE BACKGORUND ONLY UPDATE IF UPDATE THE SLIDERS.
    musicSliderBack.x = -app->render->camera.x + 900;
    musicSliderBack.y = -app->render->camera.y + 300;

    fxSliderBack.x = -app->render->camera.x + 900;
    fxSliderBack.y = -app->render->camera.y + 350;

	switch (fadeStep)
	{
	case FadeStep::NONE:
		return true;
		break;
    case FadeStep::WAIT1:
        fadeStep = FadeStep::TO_BLACK;
        break;
	case FadeStep::TO_BLACK:
		alpha += speed * dt;
		if (alpha > 255)
		{
			alpha = 255;
			fadeStep = FadeStep::WAIT2;

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
	case FadeStep::WAIT2:
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
    if (settingsEnabled)
    {
        app->render->DrawTexture(settingsPost, -app->render->camera.x + 875, -app->render->camera.y + 100, NULL);
        fullScreen->Draw(app->render);
        app->render->DrawRectangle(musicSliderBack, 255, 0, 255, 255);
        app->render->DrawRectangle(fxSliderBack, 255, 0, 255, 255);
        musicSlider->Draw(app->render);
        fxSlider->Draw(app->render);
    }
    if (pauseCondition)
    {
        app->render->DrawTexture(pauseMenu, -app->render->camera.x, -app->render->camera.y, NULL);
        resumeButton->Draw(app->render);
        settingsButton->Draw(app->render);
        exitButton->Draw(app->render);
       
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
	if (fadeStep == FadeStep::WAIT1) return;

	fadeStep = FadeStep::WAIT1;
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
	case BATTLE: next_scene = new BattleScene; break;
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
            app->hud->bagEnabled = true;
        }
        if (control->id == 122)
        {
            app->hud->bagEnabled = false;
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
        if (control->id == 100)
        {
            //MusicVolume
            
            if (app->sceneManager->musicSliderBack.w > 0)
            {
             app->audio->ChangeMusicVolume(100 * (control->bounds.x - app->sceneManager->musicSliderBack.x) / app->sceneManager->musicSliderBack.w);
            }
           
        }
        else if (control->id == 101)
        {
         if (app->sceneManager->fxSliderBack.w > 0)
            app->audio->ChangeFxVolume(100 * (control->bounds.x - app->sceneManager->fxSliderBack.x) / app->sceneManager->fxSliderBack.w);
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

void SceneManager::OnMouseAboveButton(GuiControlState state, uint32 id)
{
    if (state == GuiControlState::NORMAL)
    {
        app->hud->pencilEnabled = false;
        app->hud->calculatorEnabled = false;
        app->hud->ballEnabled = false;
        app->hud->bookEnabled = false;
        app->hud->coffeeEnabled = false;
        app->hud->colaEnabled = false;
        app->hud->eraserEnabled = false;
        app->hud->ruleEnabled = false;
        app->hud->snackEnabled = false;
        app->hud->tipexEnabled = false;
    }
    else if (state == GuiControlState::FOCUSED)
    {
    if (id == 130)
    {
        char pencilText[80] = { 0 };
        sprintf_s(pencilText, 80, "Un lapiz normal y corriente, solo recuerda, no te lo metas por la nariz.");
        app->render->DrawText(app->render->font, pencilText, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char pencilText2[80] = { 0 };
        sprintf_s(pencilText2, 80, "Inflige dano leve a un adversario.");
        app->render->DrawText(app->render->font, pencilText2, 209, 630, 40, 0, { 255, 0, 0, 255 });
    }
    else if (id == 131)
    {
        char ballText[80] = { 0 };
        sprintf_s(ballText, 80, "Parece que esta triste... y realmente lo esta.");
        app->render->DrawText(app->render->font, ballText, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char ballText2[80] = { 0 };
        sprintf_s(ballText2, 80, "Inflige dano moderado a un enemigo y lo paraliza durante 3 turnos.");
        app->render->DrawText(app->render->font, ballText2, 209, 630, 40, 0, { 255, 0, 0, 255 });
    }
    else if (id == 132)
    {
        char bookText[100] = { 0 };
        sprintf_s(bookText, 100, "Esta un poco roto pero se puede leer que 'Ya esta disponible en todas las librerias de...'");
        app->render->DrawText(app->render->font, bookText, 209, 588, 35, 0, { 0, 0, 0, 255 });

        char bookText2[80] = { 0 };
        sprintf_s(bookText2, 80, "Solo se puede usar para combinar.");
        app->render->DrawText(app->render->font, bookText2, 209, 630, 35, 0, { 255, 0, 0, 255 });
    }
    else if (id == 133)
    {
        char calculatorText[80] = { 0 };
        sprintf_s(calculatorText, 80, "A dia de hoy la sigues usando para hacer operaciones de menos de dos digitos.");
        app->render->DrawText(app->render->font, calculatorText, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char calculatorText2[80] = { 0 };
        sprintf_s(calculatorText2, 80, "Causa paralisis durante 2 turnos a un adversario.");
        app->render->DrawText(app->render->font, calculatorText2, 209, 630, 40, 0, { 255, 0, 0, 255 });
    }
    else if (id == 134)
    {
        char coffeText[80] = { 0 };
        sprintf_s(coffeText, 80, "Sin cafeina, no queremos a ninos rebeldes.");
        app->render->DrawText(app->render->font, coffeText, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char coffeText2[80] = { 0 };
        sprintf_s(coffeText2, 80, "Aumenta la velocidad de la barra de ATB.");
        app->render->DrawText(app->render->font, coffeText2, 209, 630, 40, 0, { 255, 0, 0, 255 });
    }
    else if (id == 135)
    {
        char colaText[80] = { 0 };
        sprintf_s(colaText, 80, "Siempre limpia la parte superior antes de beber de ella, no queremos mas bajas.");
        app->render->DrawText(app->render->font, colaText, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char colaText2[80] = { 0 };
        sprintf_s(colaText2, 80, "Solo se puede usar para combinar.");
        app->render->DrawText(app->render->font, colaText2, 209, 630, 40, 0, { 255, 0, 0, 255 });
    }
    else if (id == 136)
    {
        char eraserText[90] = { 0 };
        sprintf_s(eraserText, 90, "Aun hay marcas de alguien que ha intentado borrar un dibujo hecho con boligrafo.");
        app->render->DrawText(app->render->font, eraserText, 209, 588, 35, 0, { 0, 0, 0, 255 });

        char eraserText2[80] = { 0 };
        sprintf_s(eraserText2, 80, "Baja el ataque de un adversario.");
        app->render->DrawText(app->render->font, eraserText2, 209, 630, 35, 0, { 255, 0, 0, 255 });
    }
    else if (id == 137)
    {
        char ruleText[80] = { 0 };
        sprintf_s(ruleText, 80, "Con 20 centimetros son mas que suficientes.");
        app->render->DrawText(app->render->font, ruleText, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char ruleText2[80] = { 0 };
        sprintf_s(ruleText2, 80, "Inflige dano moderado a un adversario.");
        app->render->DrawText(app->render->font, ruleText2, 209, 630, 40, 0, { 255, 0, 0, 255 });
    }
   else if (id == 138)
    {
    char snackText[200] = { 0 };
    sprintf_s(snackText, 200, "9 de cada 10 nutricionistas no recomiendan comer entre horas, el otro no quiere sentirse mal con si mismo.");
    app->render->DrawText(app->render->font, snackText, 209, 588, 30, 0, { 0, 0, 0, 255 });

    char snackText2[80] = { 0 };
    sprintf_s(snackText2, 80, "Aumenta el dano del jugador durante 2 turnos.");
    app->render->DrawText(app->render->font, snackText2, 209, 630, 40, 0, { 255, 0, 0, 255 });
    }
   else if (id == 139)
   {
    char tipexEnabled[80] = { 0 };
    sprintf_s(tipexEnabled, 80, "Todos sabemos que su nombre es Tipex por mas que intentemos negarlo.");
    app->render->DrawText(app->render->font, tipexEnabled, 209, 588, 40, 0, { 0, 0, 0, 255 });

    char tipexEnabled2[80] = { 0 };
    sprintf_s(tipexEnabled2, 80, "El siguiente ataque del adversario fallara.");
    app->render->DrawText(app->render->font, tipexEnabled2, 209, 630, 40, 0, { 255, 0, 0, 255 });
   }
    }
}
