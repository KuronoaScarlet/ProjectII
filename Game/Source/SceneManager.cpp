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
#include "EntityManager.h"
#include "ParticlesEngine.h"

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{

}

bool SceneManager::Start(SceneType type)
{
    //SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

    ChangeScene(type, 0);

    //PAUSE.................................................................
    pauseMenu = app->tex->Load("Assets/Textures/Screens/pause_screen.png");
    trans1 = app->tex->Load("Assets/Textures/Screens/loading_screen.png");;
    trans2 = app->tex->Load("Assets/Textures/Screens/loading_screen.png");;

    settingsPost = app->tex->Load("Assets/Textures/postit.png");

    music_s = app->tex->Load("Assets/Textures/fx_s.png");
    fx_s = app->tex->Load("Assets/Textures/music_s.png");

    resumeButton = new GuiButton(300, { 517,304, 240, 60 }, "CONTINUE");
    resumeButton->SetObserver(this);
    resumeButton->SetTexture(app->tex->Load("Assets/Textures/resume.png"), app->tex->Load("Assets/Textures/resume_selected.png"), app->tex->Load("Assets/Textures/resume_pressed.png"));

    settingsButton = new GuiButton(301, { 517, 409, 234, 55 }, "SETTINGS");
    settingsButton->SetObserver(this);
    settingsButton->SetTexture(app->tex->Load("Assets/Textures/settings.png"), app->tex->Load("Assets/Textures/settings_selected.png"), app->tex->Load("Assets/Textures/settings_pressed.png"));

    fullScreen = new GuiCheckBox(302, { 620,400, 300, 60 }, "FULLSCREEN");
    fullScreen->SetObserver(this);
    fullScreen->SetTexture(app->tex->Load("Assets/Textures/fs1.png"), app->tex->Load("Assets/Textures/fs2.png"), app->tex->Load("Assets/Textures/fs2.png"));
    vSync = new GuiCheckBox(303, { 620,400, 300, 60 }, "VSYNC");
    vSync->SetObserver(this);
    vSync->SetTexture(app->tex->Load("Assets/Textures/vs1.png"), app->tex->Load("Assets/Textures/vs2.png"), app->tex->Load("Assets/Textures/vs2.png"));

    musicSlider = new GuiSlider(304, { 1020,300, 60, 60 }, "FULLSCREEN");
    musicSlider->SetObserver(this);
    musicSlider->SetTexture(app->tex->Load("Assets/Textures/fx.png"), app->tex->Load("Assets/Textures/fx_selected.png"), app->tex->Load("Assets/Textures/fx_focused.png"));

    fxSlider = new GuiSlider(305, { 1020,350, 60, 60 }, "FULLSCREEN");
    fxSlider->SetObserver(this);
    fxSlider->SetTexture(app->tex->Load("Assets/Textures/fx.png"), app->tex->Load("Assets/Textures/fx_selected.png"), app->tex->Load("Assets/Textures/fx_focused.png"));

    exitButton = new GuiButton(306, { 551, 360, 172, 55 }, "CREDITS");
    exitButton->SetObserver(this);
    exitButton->SetTexture(app->tex->Load("Assets/Textures/exit.png"), app->tex->Load("Assets/Textures/exit_selected.png"), app->tex->Load("Assets/Textures/exit_pressed.png"));

    musicSliderBack = { 900,300,300,40 };
    fxSliderBack = { 900,350,300,40 };


    currentIteration = 0;
    totalIterations = 120;

    back_quests = app->tex->Load("Assets/Textures/Screens/inventory.png");
    done_quests = app->tex->Load("Assets/Textures/music_s.png");
    todo_quests = app->tex->Load("Assets/Textures/music_n.png");

    return false;
}

bool SceneManager::Update(float dt)
{
	if (scene) scene->Update(dt);


    if (pauseCondition)
    {
        resumeButton->Update(app->input, dt);
        settingsButton->Update(app->input, dt);
        exitButton->Update(app->input, dt);
    }
    if (settingsEnabled)
    {
        musicSlider->Update(app->input, dt);
        fxSlider->Update(app->input, dt);
        fullScreen->Update(app->input, dt);
        vSync->Update(app->input, dt);
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
    fullScreen->bounds.y = -app->render->camera.y + 100;

    vSync->bounds.x = -app->render->camera.x + 900;
    vSync->bounds.y = -app->render->camera.y + 350;

   // musicSlider->bounds.x = -app->render->camera.x + 900;
    musicSlider->bounds.y = -app->render->camera.y + 220;

    //fxSlider->bounds.x = -app->render->camera.x + 900;
    fxSlider->bounds.y = -app->render->camera.y + 300;

    //RECT OF THE BACKGORUND ONLY UPDATE IF UPDATE THE SLIDERS.
    musicSliderBack.x = -app->render->camera.x + 935;
    musicSliderBack.y = -app->render->camera.y + 220;

    fxSliderBack.x = -app->render->camera.x + 935;
    fxSliderBack.y = -app->render->camera.y + 300;

	switch (fadeStep)
	{
	case FadeStep::NONE:
		return true;
		break;
    case FadeStep::WAIT1:
        fadeStep = FadeStep::TO_BLACK;
        break;
	case FadeStep::TO_BLACK:
        if (transId == 0)
        {
            alpha += speed * dt;
            if (alpha > 255)
            {
                alpha = 255;
                fadeStep = FadeStep::WAIT2;

                if (scene) scene->CleanUp();

                delete scene;
                scene = next_scene;
                next_scene = nullptr;

                scene->active = true;
                //scene->Awake();
                scene->Init();
                scene->Start();
            }
        }
        else if (transId == 1)
        {
            positionX = easing->backEaseInOut(currentIteration, -1500, 1500, totalIterations);

            if (currentIteration < totalIterations)
            {
                ++currentIteration;
            }
            else
            {
                fadeStep = FadeStep::WAIT2;

                if (scene) scene->CleanUp();

                delete scene;
                scene = next_scene;
                next_scene = nullptr;

                scene->active = true;
                //scene->Awake();
                scene->Init();
                scene->Start();
            }
        }
        else if (transId == 2)
        {
            totalIterations = 60;
            ++currentIteration;
            
            if (currentIteration < totalIterations)
            {
                positionY = easing->linearEaseNone(currentIteration, -750, 750, totalIterations);
            }
            if (currentIteration < totalIterations*2)
            {
                positionY2 = easing->linearEaseNone(currentIteration, 750*2, -750*2, totalIterations*2);
            }
            if (currentIteration < totalIterations*3)
            {
                positionY3 = easing->linearEaseNone(currentIteration, -750*3, 750*3, totalIterations*3);
            }
            if (currentIteration < totalIterations*4)
            {
                positionY4 = easing->linearEaseNone(currentIteration, 750*4, -750*4, totalIterations*4);
            }
            else
            {
                fadeStep = FadeStep::WAIT2;

                if (scene) scene->CleanUp();

                delete scene;
                scene = next_scene;
                next_scene = nullptr;

                scene->active = true;
                //scene->Awake();
                scene->Init();
                scene->Start();
            }
        }
		break;
	case FadeStep::WAIT2:
		fadeStep = FadeStep::FROM_BLACK;
        currentIteration = 0;
		break;
	case FadeStep::FROM_BLACK:
        if (transId == 0)
        {
            alpha -= speed * dt;
            if (alpha < 0)
            {
                alpha = 0;
                fadeStep = FadeStep::NONE;
            }
        }
        else if (transId == 1)
        {
            positionX = easing->sineEaseIn(currentIteration, 0, 1500, totalIterations);

            if (currentIteration < totalIterations)
            {
                ++currentIteration;
            }
            else
            {
                fadeStep = FadeStep::NONE;
                currentIteration = 0;

            }
        }
        else if (transId == 2)
        {
            ++currentIteration;

            if (currentIteration < totalIterations)
            {
                positionY = easing->linearEaseNone(currentIteration, 0, 750, totalIterations);
            }
            if (currentIteration < totalIterations * 2)
            {
                positionY2 = easing->linearEaseNone(currentIteration, 0, -750 * 2, totalIterations);
            }
            if (currentIteration < totalIterations * 3)
            {
                positionY3 = easing->linearEaseNone(currentIteration, 0, 750 * 3, totalIterations);
            }
            if (currentIteration < totalIterations * 4)
            {
                positionY4 = easing->linearEaseNone(currentIteration, 0, -750 * 4, totalIterations);
            }
                       
            else
            {
                fadeStep = FadeStep::NONE;
                currentIteration = 0;

            }
        }
		break;
	}
    
	return true;
}

bool SceneManager::PostUpdate()
{
	if (scene) scene->PostUpdate();

    if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
    {
        DisplayQuests();
    }

    if (newQuestAdded != 0)
    {
        ListItem<Quest*>* quest = quests.start;
        while (quest)
        {
            if (quest->data->id == newQuestAdded)
            {
                if (currentIterationQuest < 360) ++currentIterationQuest;
                else
                {
                    currentIterationQuest = 0;
                    newQuestAdded = 0;
                    app->particleSystem->emitters->Clear();
                }
                int posX = easing->bounceEaseOut(currentIterationQuest, -200, 400, 360);
                app->render->DrawText(app->render->font, quest->data->text, posX, 200, 75, 0, { 255, 255, 0, 255 });
                app->render->DrawTexture(done_quests, 150 - app->render->camera.x, 200 - app->render->camera.y);
                break;
            }
            quest = quest->next;
        }
    }

    if (transId == 0)
    {
        SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, alpha);
        SDL_Rect screen{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        SDL_RenderFillRect(app->render->renderer, &screen);
    }
    else if (transId == 1)
    {
        app->render->DrawTexture(trans1, -app->render->camera.x + positionX, -app->render->camera.y, NULL);
    }
    else if (transId == 2)
    {
        SDL_Rect pos1{ 0,0,320,720 };
        SDL_Rect pos2{ 320,0,320,720 };
        SDL_Rect pos3{ 640,0,320,720 };
        SDL_Rect pos4{ 960,0,320,720 };

        app->render->DrawTexture(trans1, -app->render->camera.x + 0, -app->render->camera.y + positionY + 12, &pos1);
        app->render->DrawTexture(trans1, -app->render->camera.x + 320, -app->render->camera.y + positionY2-12, &pos2);
        app->render->DrawTexture(trans1, -app->render->camera.x + 640, -app->render->camera.y + positionY3+12, &pos3);
        app->render->DrawTexture(trans1, -app->render->camera.x + 960, -app->render->camera.y + positionY4-12, &pos4);
    }
    if (settingsEnabled)
    {
        app->render->DrawTexture(settingsPost, -app->render->camera.x + 875, -app->render->camera.y + 100, NULL);
        fullScreen->Draw(app->render);
        vSync->Draw(app->render);
        app->render->DrawRectangle(musicSliderBack, 192, 192, 192, 255);
        app->render->DrawRectangle(fxSliderBack, 192, 192, 192, 255);
        musicSlider->Draw(app->render);
        fxSlider->Draw(app->render);
        app->render->DrawTexture(fx_s, -app->render->camera.x + 895, -app->render->camera.y + 222, NULL);
        app->render->DrawTexture(music_s, -app->render->camera.x + 895, -app->render->camera.y + 302, NULL);
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

void SceneManager::ChangeScene(SceneType type, int Id, float new_speed)
{
	if (fadeStep == FadeStep::WAIT1) return;

	fadeStep = FadeStep::WAIT1;
	speed = new_speed;
	alpha = 0;
    transId = Id;
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
        if (control->id == 121)
        {
            app->hud->statsEnabled = true;
        }
        if (control->id == 122)
        {
            app->hud->bagEnabled = false;
            app->hud->statsEnabled = false;
        }
        if (control->id == 101)
        {
            atkMenu = true;
        }
        if (control->id == 102)
        {
            defMenu = true;
        }
        if (control->id == 103)
        {

        }
        if (control->id == 104)
        {
            if(app->entityManager->playerData.scene == 1) app->sceneManager->ChangeScene(SCENE1,0);
            if(app->entityManager->playerData.scene == 2) app->sceneManager->ChangeScene(SCENE12,0);
        }
        if (control->id == 13)
        {
            creditsOnScreen = true;
        }
        if (control->id == 503)
        {
            creditsOnScreen = false;
            creditSceneFlag = false;
        }
        if (control->id == 510)
        {
            creditsOnScreen = false;
            creditSceneFlag = false;
        }
        if (control->id == 301)
        {
            //Play
           settingsEnabled = !settingsEnabled;
        }
        if (control->id == 501)
        {
            //LoadGame
            app->loadingGame = true;
            pugi::xml_document savedGame;
            savedGame.load_file("save_game.xml");

            pugi::xml_node generalNode = savedGame.child("save");
            pugi::xml_node map = generalNode.child("map");
            app->map->LoadState(map);

            if (app->currentLevel == 1) ChangeScene(SCENE1,2);
            if (app->currentLevel == 2) ChangeScene(SCENE12,2);
        }
        else if (control->id == 504)
        {
            settingsEnabled = !settingsEnabled;
        }
        else if (control->id == 306)
        {
            pauseCondition = false;

            //Back to title
            ChangeScene(TITLE,0);
        }
        else if (control->id == 509)
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
        else if (control->id == 502)
        {
            ChangeScene(INTRO,0);
        }
        else if (control->id == 505)
        {
            creditSceneFlag = true;
        }
        else if (control->id == 300)
        {
            pauseCondition = false;
        }
        else if (control->id == 140)
        {
            if (app->entityManager->playerData.Pencil > 0 && app->entityManager->playerData.Sharper > 0)
            {
                app->entityManager->playerData.pencilSharpened++;
                app->entityManager->playerData.Pencil--;
                app->entityManager->playerData.Sharper--;
                CompleteQuest(1);
            }
        }
        else if (control->id == 141)
        {
            if (app->entityManager->playerData.coffee > 0 && app->entityManager->playerData.cola > 0)
            {
                app->entityManager->playerData.wonster++;
                app->entityManager->playerData.coffee--;
                app->entityManager->playerData.cola--;
                CompleteQuest(1);
            }
        }
    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 304)
        {
            //MusicVolume
            
            if (app->sceneManager->musicSliderBack.w > 0)
            {
             app->audio->ChangeMusicVolume(100 * (control->bounds.x - app->sceneManager->musicSliderBack.x) / app->sceneManager->musicSliderBack.w);
            }
        }
        else if (control->id == 305)
        {
         if (app->sceneManager->fxSliderBack.w > 0)
            app->audio->ChangeFxVolume(100 * (control->bounds.x - app->sceneManager->fxSliderBack.x) / app->sceneManager->fxSliderBack.w);
        }
        if (control->id == 507)
        {
            if (app->sceneManager->musicSliderBack.w > 0)
            {
                app->audio->ChangeMusicVolume(100 * (control->bounds.x - app->sceneManager->musicSliderBack.x) / app->sceneManager->musicSliderBack.w);
            }
        }
        else if (control->id == 508)
        {
            if (app->sceneManager->fxSliderBack.w > 0)
                app->audio->ChangeFxVolume(100 * (control->bounds.x - app->sceneManager->fxSliderBack.x) / app->sceneManager->fxSliderBack.w);
        }
    }
    case GuiControlType::CHECKBOX:
    {
        if (control->id == 302)
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
        if (control->id == 506)
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
  else if (id == 140)
   {
        char sharpedPencilEnabled[80] = { 0 };
        sprintf_s(sharpedPencilEnabled, 80, "Ni Jack el destripador tenía un arma tan afilada como esta. Lapiz + Sacapuntas");
        app->render->DrawText(app->render->font, sharpedPencilEnabled, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char sharpedPencilEnabled2[80] = { 0 };
        sprintf_s(sharpedPencilEnabled2, 80, "Inflige daño moderado a un adversario y reduce su defensa durante 2 turnos.");
        app->render->DrawText(app->render->font, sharpedPencilEnabled2, 209, 630, 40, 0, { 255, 0, 0, 255 });
   }
   else if (id == 141)
   {
        char wonsterEnabled[200] = { 0 };
        sprintf_s(wonsterEnabled, 200, "Las leyes del copyright no tienen efecto contra ti. Cafe + Refresco");
        app->render->DrawText(app->render->font, wonsterEnabled, 209, 588, 40, 0, { 0, 0, 0, 255 });

        char wonsterEnabled2[200] = { 0 };
        sprintf_s(wonsterEnabled2, 200, "Aumenta considerablemente el ataque y la velocidad de la barra ATB durante 3 turnos.");
        app->render->DrawText(app->render->font, wonsterEnabled2, 209, 630, 40, 0, { 255, 0, 0, 255 });
   }
    }
}

void SceneManager::CreateQuest(int _id, const char* _text)
{
    ListItem<Quest*>* quest = quests.start;
    while (quest)
    {
        if (quest->data->id == _id) return;
        quest = quest->next;
    }
    Quest* newQuest = new Quest;
    newQuest->id = _id;
    newQuest->text = _text;
    newQuest->completed = false;
    quests.Add(newQuest);
    newQuestAdded = newQuest->id;

    app->particleSystem->AddEmitter(EmitterType::FIRE, app->entityManager->playerData.position.x-app->render->camera.x, app->entityManager->playerData.position.x - app->render->camera.y);
    //app->particleSystem->AddEmitter(EmitterType::FIRE, 200, 200);
}

void SceneManager::CompleteQuest(int _id)
{
    ListItem<Quest*>* quest = quests.start;
    while (quest)
    {
        if (quest->data->id == _id)
        {
            quest->data->completed = true;
            return;
        }
        quest = quest->next;
    }
}

void SceneManager::DisplayQuests()
{
    app->render->DrawTexture(back_quests, 50 - app->render->camera.x, 50 - app->render->camera.y);
    int posY = 100;
    ListItem<Quest*>* quest = quests.start;
    while (quest)
    {
        app->render->DrawText(app->render->font, quest->data->text, 200, posY, 50, 0, { 255, 0, 255, 255 });
        if (quest->data->completed)
            app->render->DrawTexture(done_quests, 150 - app->render->camera.x, posY - app->render->camera.y);
        else
            app->render->DrawTexture(todo_quests, 150 - app->render->camera.x, posY - app->render->camera.y);
        posY += 100;
        quest = quest->next;
    }
}
