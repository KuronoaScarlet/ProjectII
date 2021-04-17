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
    app->audio->PlayMusic("Assets/Audio/Music/menu_music.ogg");


    play = new GuiButton(1, { 517, 304, 240, 60 }, "CONTINUE");
    play->SetObserver((Scene1*)this);
    play->SetTexture(app->tex->Load("Assets/Textures/continue.png"), app->tex->Load("Assets/Textures/continue_selected.png"), app->tex->Load("Assets/Textures/continue_pressed.png"));
    play->SetDisableTexture(app->tex->Load("Assets/Textures/continue_disabled.png"));
    if (!app->fileSaved)
    {
       play->state = GuiControlState::DISABLED;
    }
    
    newGame = new GuiButton(12, { 517, 370, 234, 55 }, "START");
    newGame->SetObserver((Scene1*)this);
    newGame->SetDisableTexture(app->tex->Load("Assets/Textures/Buttons/states/no.png"));
    newGame->SetTexture(app->tex->Load("Assets/Textures/newgame.png"), app->tex->Load("Assets/Textures/newgame_selected.png"), app->tex->Load("Assets/Textures/newgame_pressed.png"));
    
    options = new GuiButton(2, { 543, 438, 197, 55 }, "OPTIONS");
    options->SetObserver((Scene1*)this);
    options->SetTexture(app->tex->Load("Assets/Textures/settings.png"), app->tex->Load("Assets/Textures/settings_selected.png"), app->tex->Load("Assets/Textures/settings_pressed.png"));

    credits = new GuiButton(13, { 551, 514, 172, 55 }, "CREDITS");
    credits->SetObserver((Scene1*)this);
    credits->SetTexture(app->tex->Load("Assets/Textures/credits.png"), app->tex->Load("Assets/Textures/credits_selected.png"), app->tex->Load("Assets/Textures/credits_pressed.png"));

    exit = new GuiButton(4, { 580, 569, 117, 55 }, "EXIT");
    exit->SetObserver((Scene1*)this);
    exit->SetTexture(app->tex->Load("Assets/Textures/exit.png"), app->tex->Load("Assets/Textures/exit_selected.png"), app->tex->Load("Assets/Textures/exit_pressed.png"));

    backButton = new GuiButton(3, { 220, 375, 100, 55 }, "BACK");
    backButton->SetObserver((Scene1*)this);
    backButton->SetTexture(app->tex->Load("Assets/Textures/Buttons/states/play.png"), app->tex->Load("Assets/Textures/Buttons/states/focused.png"), app->tex->Load("Assets/Textures/Buttons/states/pressed.png"));

    creditsScene = app->tex->Load("Assets/Textures/credits_scene.png");
    creditSceneFlag = false;
    pauseBool = false;
    fullSc = false;
    vsync = true;
    exi = false;

    app->render->camera.x = 0;
    //app->render->camera.y = 1000000000; XD, THIS DOES NOTHING

   return ret;
}

bool Title::PreUpdate()
{
    return true;
}

bool Title::Update(float dt)
{
    if (creditSceneFlag == false)
    {
        play->Update(app->input, dt);
        newGame->Update(app->input, dt);
        options->Update(app->input, dt);
        credits->Update(app->input, dt);
        exit->Update(app->input, dt);
    }
    if (creditSceneFlag)
    {
        app->render->camera.y -= 1;
        
        
    }
    if (app->render->camera.y == -1000) //-2400
    {
        creditSceneFlag = false;
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
        app->fade->Fade(this, (Module*)app->scene1, 10);

    }
   // if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    
    if (creditSceneFlag == true)
    {
        app->render->DrawTexture(creditsScene, 0, 0, NULL);
    }

    if (!creditSceneFlag)
    
    {
        app->render->camera.y = 0;
        app->render->DrawTexture(screen, 0, 0, NULL);


        // start->Draw(app->render);
        // SDL_Rect rectPlayer = playerData.currentAnim->GetCurrentFrame();
        play->Draw(app->render);
        app->render->DrawText(app->render->font, "Play", 600, 239, 60, 5, { 255, 255, 255, 255 });
        if (!app->fileSaved)
        {
            app->render->DrawText(app->render->font, "Load", 600, 328, 60, 5, { 255, 255, 255, 255 });
            newGame->Draw(app->render);
        }
        else
        {
            newGame->Draw(app->render);
            app->render->DrawText(app->render->font, "Load", 600, 328, 60, 5, { 255, 255, 255, 255 });
        }
        options->Draw(app->render);
        app->render->DrawText(app->render->font, "Settings", 550, 418, 60, 5, { 255, 255, 255, 255 });
        credits->Draw(app->render);
        app->render->DrawText(app->render->font, "Credits", 560, 508, 60, 5, { 255, 255, 255, 255 });
        exit->Draw(app->render);
        app->render->DrawText(app->render->font, "Exit", 610, 597, 60, 5, { 255, 255, 255, 255 });
    }
   
    
    

    if (exi == true) 
    {
        return false;
    }
    
    

    return ret;
}

bool Title::CleanUp()
{
    if (!active)return true;
    LOG("Freeing Logo");
    app->title->active = false;
    app->tex->UnLoad(screen);
    return true;
}

bool Scene1::OnGuiMouseClickEvent(GuiControl* control)
{
    

    switch (control->type)
    {
    case GuiControlType::BUTTON:
    {
        if (control->id == 17)
        {
            //Play
            app->entityManager->settingsEnabled = !app->entityManager->settingsEnabled;
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

            if (app->currentLevel == 1) app->fade->Fade((Module*)app->title, (Module*)app->scene1, 10);
           
        }
        else if (control->id == 2)
        {
            //Settings
            
        }
        else if (control->id == 3)
        {
                       
        }
        else if (control->id == 11)
        {
            app->entityManager->playerData.pauseCondition = false;

            //Back to title
            if (app->scene1->active == true)
            {
                app->fade->Fade((Module*)app->scene1, (Module*)app->title, 30);
            }
        }

        else if (control->id == 4)
        {
            //Exit
            app->title->exi = true;
        }
       else if (control->id == 8)
        {
            //Vsync
            if (app->title->vsync == true)
            {
                
                app->title->vsync = false;
            }
            else
            {
               
            }
        }
        else if (control->id == 9)
        {
                       
        }
        else if (control->id == 12)
        {
            app->fade->Fade((Module*)app->title, (Module*)app->intro, 20);
        }
        else if (control->id == 13)
        {
            app->title->creditSceneFlag = true;
        }

        else if (control->id == 32)
        {
            app->entityManager->playerData.pauseCondition = false;
        }
 
    }
    case GuiControlType::SLIDER:
    {
        if (control->id == 5)
        {
            //Volume
            if(control->bounds.x == 143 || control->bounds.x == 156.5f|| control->bounds.x == 170|| control->bounds.x == 183.5f|| control->bounds.x == 197|| control->bounds.x == 210.5f|| control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
            {
                app->title->volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
                app->audio->Volume(app->title->volumMusic, '0');
                
                
            }
            
        }
        else if (control->id == 6)
        {
            //FxVolume
            if (control->bounds.x == 143 || control->bounds.x == 156.5f || control->bounds.x == 170 || control->bounds.x == 183.5f || control->bounds.x == 197 || control->bounds.x == 210.5f || control->bounds.x == 224 || control->bounds.x == 237.5f || control->bounds.x == 251 || control->bounds.x == 264.5f || control->bounds.x == 278)
            {
                app->title->volumMusic = ((control->bounds.x - 143) / 13.5) * 10;
                app->audio->Volume(app->title->volumMusic, '1');
                

            }

        }

    }
    case GuiControlType::CHECKBOX:
    {
        if (control->id == 7)
        {
            //FullScreen
            if (app->title->fullSc == false)
            {
                SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
                app->title->fullSc = true;
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