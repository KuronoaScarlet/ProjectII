#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene1.h"
#include "Map.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Title.h"
#include "HUD.h"
#include "DialogSystem.h"

#include "Defs.h"
#include "Log.h"

Hud::Hud() : Module()
{
	name.Create("scene");
}

// Destructor
Hud::~Hud()
{}

// Called before render is available
bool Hud::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Hud::Start()
{
	active = true;
	app->hud->active = true;
	bag = new GuiButton(120, { 1155,20, 90, 125 }, "BAG");// 1155,20
	bag->SetObserver(this);
	bag->SetTexture(app->tex->Load("Assets/Textures/bag.png"), app->tex->Load("Assets/Textures/bag2.png"), app->tex->Load("Assets/Textures/bag2.png"));

	pencil = new GuiButton(130, { 169,82, 69, 72 }, "PENCIL");// 1155,20
	pencil->SetObserver(this);
	pencil->SetTexture(app->tex->Load("Assets/Textures/Items/Pencil.png"), app->tex->Load("Assets/Textures/Items/Pencil.png"), app->tex->Load("Assets/Textures/Items/Pencil.png"));

	inventoryTab = app->tex->Load("Assets/Textures/inventory_tab.png");

	return true;
}

// Called each loop iteration
bool Hud::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Hud::Update(float dt)
{
	if (!app->entityManager->settingsEnabled)
	{
		app->hud->bag->Update(app->input, dt);
	}
	if (!app->entityManager->settingsEnabled && app->hud->bagEnabled)
	{
		app->hud->pencil->Update(app->input, dt);
	}

	return true;
}

// Called each loop iteration
bool Hud::PostUpdate()
{
	bag->bounds.x = -app->render->camera.x + 1155;
	bag->bounds.y = -app->render->camera.y + 20;

	pencil->bounds.x = -app->render->camera.x + 169;
	pencil->bounds.y = -app->render->camera.y + 92;
	bool ret = true;

	
	if (!app->entityManager->settingsEnabled && app->scene1->active)
	{
		bag->Draw(app->render);
	}
	if (app->hud->bagEnabled && !app->entityManager->settingsEnabled && app->scene1->active)
	{
		app->render->DrawTexture(inventoryTab, -app->render->camera.x + 0, -app->render->camera.y + 0, NULL);
	}
	if (app->hud->bagEnabled && !app->entityManager->settingsEnabled && app->scene1->active)
	{
		pencil->Draw(app->render);
	}

	return ret;
}

// Called before quitting
bool Hud::CleanUp()
{
	app->hud->active = false;
	if (!active)return true;

	app->entityManager->CleanUp();

	LOG("Freeing scene");
	return true;
}