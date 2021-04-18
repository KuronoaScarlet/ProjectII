#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Animation.h"
#include "WinScreen.h"
#include "Scene1.h"
#include "FadeToBlack.h"
#include "EntityManager.h"


#include "Defs.h"
#include "Log.h"

WinScreen::WinScreen() : Module()
{
    name.Create("winscreen");
}

WinScreen::~WinScreen()
{

}

bool WinScreen::Awake()
{
    LOG("Loading Screens");
    bool ret = true;

    return ret;
}

// Load assets
bool WinScreen::Start()
{
    LOG("Loading Screens assets");
    timer.Start();
    bool ret = true;
    app->entityManager->winCount = 0;
    screen = app->tex->Load("Assets/Textures/Screens/win_screen.png");
    app->audio->PlayMusic("Assets/Audio/Music/win_scene_music.ogg");

    return ret;
}

bool WinScreen::PreUpdate()
{
    return true;
}

bool WinScreen::Update(float dt)
{
    return true;
}

// Update: draw background
bool WinScreen::PostUpdate()
{


    finalTimer = count - timer.ReadSec();
    if (finalTimer == 0)
    {
        app->fade->Fade(this, (Module*)app->title);
        //finalTimer = 10;
    }
    bool ret = true;
    // Draw everything --------------------------------------
    app->render->camera.x = 0;
    app->render->camera.y = 0;
    app->render->DrawTexture(screen, 0, 0, NULL);

    return ret;
}

bool WinScreen::CleanUp()
{
    if (!active)return true;
    LOG("Freeing intro");
    app->winScreen->active = false;
    app->tex->UnLoad(screen);
    app->entityManager->CleanUp();
    app->map->CleanUp();
    return true;
}