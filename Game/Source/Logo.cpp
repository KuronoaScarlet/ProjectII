#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Animation.h"
#include "Logo.h"
#include "FadeToBlack.h"
#include "Easings.h"
#include "SceneManager.h"


#include "Defs.h"
#include "Log.h"

Logo::Logo() : Module()
{
    name.Create("Logo");
}

Logo::~Logo()
{

}

bool Logo::Awake()
{
    LOG("Loading Logo");
    bool ret = true;

    return ret;
}

// Load assets
bool Logo::Start()
{
    LOG("Loading Logo assets");

    bool ret = true;

    currentIteration = 0;
    totalIterations = 120;
    initialPosition = -1500;
    deltaPosition = 1500;

    position_x = -500;

    screen = app->tex->Load("Assets/Textures/Screens/team_logo.png");
    app->audio->PlayMusic("Assets/Audio/Music/logoscreen_fx.ogg");



    timer = 0;
    trans = true;

    return ret;
}

bool Logo::PreUpdate()
{
    return true;
}

bool Logo::Update(float dt)
{
    timer += dt;

    return true;
}

// Update: draw background
bool Logo::PostUpdate()
{
    bool ret = true;
    // Draw everything --------------------------------------
    position_x = easing->backEaseInOut(currentIteration, initialPosition, deltaPosition, totalIterations);

    if (position_x == 0.0f && trans == true)
    {
        //SDL_Delay(200);
        trans = false;
        app->sceneManager->ChangeScene(TITLE,1);
        /*CleanUp();
        app->title->Init();
        app->title->Start();*/
    }
    //if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
    if (currentIteration < totalIterations)
    {
        ++currentIteration;
    }
   app->render->DrawRectangle({ 0,0,1280, 720 }, 241, 241, 241, 255);
   app->render->DrawTexture(screen, position_x, 0, NULL);
 
    return ret;
}

bool Logo::CleanUp()
{
    if (!active)return true;
    LOG("Freeing Logo");
    active = false;
    app->tex->UnLoad(screen);
    return true;
}