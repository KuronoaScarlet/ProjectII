#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Collisions.h"
#include "Map.h"
#include "Animation.h"
#include "Intro.h"
#include "FadeToBlack.h"
#include "DialogSystem.h"
#include "EntityManager.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

Intro::Intro() : Module()
{
    name.Create("Intro");
}

Intro::~Intro()
{

}

bool Intro::Awake()
{
    LOG("Loading Intro");
    bool ret = true;

    return ret;
}

// Load assets
bool Intro::Start()
{
    LOG("Loading Intro assets");

    bool ret = true;

    return ret;
}

bool Intro::PreUpdate()
{
    return true;
}

bool Intro::Update(float dt)
{
    if (app->entityManager->playerData.onDialog == false)
    {
        app->entityManager->playerData.onDialog = true;
        app->dialogueSystem->actionChecks = 0;
        app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[3]->dialogueNodes[0];
        app->dialogueSystem->PerformDialogue(3, 7);
        app->dialogueSystem->Id = 3;
    }

    return true;
}

// Update: draw background
bool Intro::PostUpdate()
{
    bool ret = true;

    if (app->entityManager->playerData.onDialog == true)
    {
        //app->render->DrawRectangle({ -app->render->camera.x + 450,-app->render->camera.y+612,919,143 }, 255, 255, 150);

        char NPCdialogue[80] = { 0 };
        sprintf_s(NPCdialogue, 80, app->dialogueSystem->currentNode->text.c_str(), 56);
        app->render->DrawText(app->render->font, NPCdialogue, 150, 580, 45, 0, { 0, 0, 0, 255 });

        char response[80] = { 0 };
        for (int i = 0; i < app->dialogueSystem->currentNode->answersList.Count(); i++)
        {
            sprintf_s(response, 80, app->dialogueSystem->currentNode->answersList.At(i)->data.c_str(), 56);
            app->render->DrawText(app->render->font, response, 170, 588 + (25 * (i + 1)), 45, 0, { 255, 150, 150, 255 });
        }
    }

    return ret;
}

bool Intro::CleanUp()
{
    if (!active)return true;
    LOG("Freeing Logo");
    active = false;
    return true;
}