#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Animation.h"
#include "Title.h"
#include "FadeToBlack.h"
#include "Scene1.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "BattleScene.h"
#include "HUD.h"
#include "Scene12.h"
#include "SceneManager.h"


#include "Defs.h"
#include "Log.h"

Title::Title() : Module()
{
    name.Create("Logo");
}

Title::~Title()
{

}

bool Title::Awake()
{
    LOG("Loading Logo");
    bool ret = true;

    return ret;
}

// Load assets
bool Title::Start()
{
    LOG("Loading Logo assets");

    bool ret = true;

    app->hud->Init();
    app->hud->Start();
  
    currentIterationBck = 0;
    currentIterationCaronte = 0;
    currentIterationMandate = 0;
    totalIterations = 120;
    positionTitleBack = -100;
    positionTitleCaronte = -700;
    positionTitleMandate = 1280+700;

    pugi::xml_parse_result result = app->saveLoadFile.load_file("save_game.xml");
    if (result != NULL)
    {
        app->saveLoadNode = app->saveLoadFile.child("save");
        app->fileSaved = true;
    }
    if (result == NULL)
    {
        app->fileSaved = false;
    }

    screen = app->tex->Load("Assets/Textures/Screens/title_screen.png");
    bck = app->tex->Load("Assets/Textures/Screens/title_background.png");
    caronte = app->tex->Load("Assets/Textures/Screens/title_1.png");
    mandate = app->tex->Load("Assets/Textures/Screens/title_2.png");
    app->audio->PlayMusic("Assets/Audio/Music/menu_music.ogg");

    settingsPost2 = app->tex->Load("Assets/Textures/postit.png");

    play = new GuiButton(501, { 517, 304, 240, 60 }, "CONTINUE");
    play->SetObserver(this);
    play->SetTexture(app->tex->Load("Assets/Textures/continue.png"), app->tex->Load("Assets/Textures/continue_selected.png"), app->tex->Load("Assets/Textures/continue_pressed.png"));
    play->SetDisableTexture(app->tex->Load("Assets/Textures/continue_disabled.png"));

    if (!app->fileSaved)
    {
       play->state = GuiControlState::DISABLED;
    }
    
    newGame = new GuiButton(502, { 517, 370, 234, 55 }, "START");
    newGame->SetObserver(this);
    newGame->SetTexture(app->tex->Load("Assets/Textures/newgame.png"), app->tex->Load("Assets/Textures/newGame_selected.png"), app->tex->Load("Assets/Textures/newGame_pressed.png"));

    escCredits = new GuiButton(506, { 40, 20, 200, 100 }, "ESC");
    escCredits->SetObserver(this);
    escCredits->SetDisableTexture(app->tex->Load("Assets/Textures/esc.png"));
    escCredits->SetTexture(app->tex->Load("Assets/Textures/esc.png"), app->tex->Load("Assets/Textures/esc2.png"), app->tex->Load("Assets/Textures/esc3.png"));

    options = new GuiButton(503, { 543, 438, 197, 55 }, "OPTIONS");
    options->SetObserver(this);
    options->SetTexture(app->tex->Load("Assets/Textures/settings.png"), app->tex->Load("Assets/Textures/settings_selected.png"), app->tex->Load("Assets/Textures/settings_pressed.png"));

    credits = new GuiButton(504, { 551, 514, 172, 55 }, "CREDITS");
    credits->SetObserver(this);
    credits->SetTexture(app->tex->Load("Assets/Textures/credits.png"), app->tex->Load("Assets/Textures/credits_selected.png"), app->tex->Load("Assets/Textures/credits_pressed.png"));

    exit = new GuiButton(505, { 580, 569, 117, 55 }, "EXIT");
    exit->SetObserver(this);
    exit->SetTexture(app->tex->Load("Assets/Textures/exit.png"), app->tex->Load("Assets/Textures/exit_selected.png"), app->tex->Load("Assets/Textures/exit_pressed.png"));

    creditsScene = app->tex->Load("Assets/Textures/Screens/credits_screen.png");

    app->hud->creditSceneFlag = false;
    pauseBool = false;
    app->hud->fullSc = false;
    app->hud->vsync = true;
    app->hud->exi = false;

    app->render->camera.x = 0;
   
   return ret;
}

bool Title::PreUpdate()
{
    return true;
}

bool Title::Update(float dt)
{
    app->hud->pauseCondition = false;
    if (app->hud->creditSceneFlag == false && positionTitleCaronte == 430)
    {
        play->Update(app->input, dt);
        newGame->Update(app->input, dt);
        options->Update(app->input, dt);
        credits->Update(app->input, dt);
        exit->Update(app->input, dt);
    }
    if (app->hud->creditsOnScreen)
    {
        escCredits->Update(app->input, dt);
        
    }
    if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    {
        app->hud->creditSceneFlag = false;
        app->hud->creditsOnScreen = false;
    }

    positionTitleBack = easing->backEaseIn(currentIterationBck, -300, 300, totalIterations);
    if (currentIterationBck < totalIterations)
    {
        ++currentIterationBck;
    }

    positionTitleCaronte = easing->elasticEaseIn(currentIterationCaronte, -1000, 1430, totalIterations);
    if (currentIterationCaronte < totalIterations)
    {
        ++currentIterationCaronte;
    }
    positionTitleMandate = easing->elasticEaseIn(currentIterationMandate, 1500, -1070, totalIterations);
    if (currentIterationMandate < totalIterations)
    {
        ++currentIterationMandate;
    }

    return true;
}

// Update: draw background
bool Title::PostUpdate()
{
    bool ret = true;

   // Draw everything --------------------------------------
    if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        app->sceneManager->ChangeScene(SCENE1,1);
    }
    

    app->render->DrawTexture(screen, 0, 0, NULL);

    app->render->DrawTexture(bck, 200, positionTitleBack, NULL);
    app->render->DrawTexture(caronte, positionTitleCaronte, 20, NULL);//430
    app->render->DrawTexture(mandate, positionTitleMandate, 100, NULL);

    if (app->hud->creditsOnScreen)
    {
        app->render->DrawTexture(creditsScene, 0, 0, NULL);
    }

    if (app->hud->creditSceneFlag == true)
    {
        app->render->DrawTexture(creditsScene, 0, 0, NULL);
        escCredits->Draw(app->render);
        app->hud->configOn = false;
    }

    if (!app->hud->creditSceneFlag)
    {
        app->render->camera.y = 0;

        if (positionTitleCaronte == 430)
        {
            play->Draw(app->render);

            if (!app->fileSaved)
            {
                newGame->Draw(app->render);
            }
            else
            {
                newGame->Draw(app->render);
            }

            options->Draw(app->render);

            credits->Draw(app->render);

            exit->Draw(app->render);
        }
    }

    return ret;
}

bool Title::CleanUp()
{
    if (!active)return true;
    LOG("Freeing Logo");
    //play->CleanUp();
    //newGame->CleanUp();
    //escCredits->CleanUp();
    //options->CleanUp();
    //credits->CleanUp();
    //exit->CleanUp();
    active = false;
    app->tex->UnLoad(screen);
    app->tex->UnLoad(creditsScene);
    return true;
}

