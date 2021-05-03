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

	pencil = new GuiButton(130, { 204,73, 69, 72 }, "PENCIL");// 1155,20
	pencil->SetObserver(this);
	pencil->SetTexture(app->tex->Load("Assets/Textures/Items/Pencil.png"), app->tex->Load("Assets/Textures/Items/Pencil.png"), app->tex->Load("Assets/Textures/Items/Pencil.png"));

	ball = new GuiButton(131, { 617,76, 70, 70 }, "PENCIL");// 1155,20
	ball->SetObserver(this);
	ball->SetTexture(app->tex->Load("Assets/Textures/Items/ball.png"), app->tex->Load("Assets/Textures/Items/ball.png"), app->tex->Load("Assets/Textures/Items/ball.png"));

	book = new GuiButton(132, { 441,423, 47, 70 }, "PENCIL");// 1155,20
	book->SetObserver(this);
	book->SetTexture(app->tex->Load("Assets/Textures/Items/book.png"), app->tex->Load("Assets/Textures/Items/book.png"), app->tex->Load("Assets/Textures/Items/book.png"));

	calculator = new GuiButton(133, { 424,320, 71, 69 }, "PENCIL");// 1155,20
	calculator->SetObserver(this);
	calculator->SetTexture(app->tex->Load("Assets/Textures/Items/calculator.png"), app->tex->Load("Assets/Textures/Items/calculator.png"), app->tex->Load("Assets/Textures/Items/calculator.png"));

	coffee = new GuiButton(134, { 418,182, 83, 80 }, "PENCIL");// 1155,20
	coffee->SetObserver(this);
	coffee->SetTexture(app->tex->Load("Assets/Textures/Items/coffe.png"), app->tex->Load("Assets/Textures/Items/coffe.png"), app->tex->Load("Assets/Textures/Items/coffe.png"));

	cola = new GuiButton(135, { 441,76, 37, 69 }, "PENCIL");// 1155,20
	cola->SetObserver(this);
	cola->SetTexture(app->tex->Load("Assets/Textures/Items/cola.png"), app->tex->Load("Assets/Textures/Items/cola.png"), app->tex->Load("Assets/Textures/Items/cola.png"));

	eraser = new GuiButton(136, { 204,426, 64, 70 }, "PENCIL");// 1155,20
	eraser->SetObserver(this);
	eraser->SetTexture(app->tex->Load("Assets/Textures/Items/eraser.png"), app->tex->Load("Assets/Textures/Items/eraser.png"), app->tex->Load("Assets/Textures/Items/eraser.png"));

	rule = new GuiButton(137, { 204,307, 74, 74 }, "PENCIL");// 1155,20
	rule->SetObserver(this);
	rule->SetTexture(app->tex->Load("Assets/Textures/Items/rule.png"), app->tex->Load("Assets/Textures/Items/rule.png"), app->tex->Load("Assets/Textures/Items/rule.png"));

	snack = new GuiButton(138, { 204,190, 67, 72 }, "PENCIL");// 1155,20
	snack->SetObserver(this);
	snack->SetTexture(app->tex->Load("Assets/Textures/Items/snack.png"), app->tex->Load("Assets/Textures/Items/snack.png"), app->tex->Load("Assets/Textures/Items/snack.png"));

	tipex = new GuiButton(139, { 629,193, 46, 80 }, "PENCIL");// 1155,20
	tipex->SetObserver(this);
	tipex->SetTexture(app->tex->Load("Assets/Textures/Items/tipex.png"), app->tex->Load("Assets/Textures/Items/tipex.png"), app->tex->Load("Assets/Textures/Items/tipex.png"));


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
		app->hud->ball->Update(app->input, dt);
		app->hud->calculator->Update(app->input, dt);
		app->hud->book->Update(app->input, dt);
		app->hud->coffee->Update(app->input, dt);
		app->hud->cola->Update(app->input, dt);
		app->hud->eraser->Update(app->input, dt);
		app->hud->rule->Update(app->input, dt);
		app->hud->snack->Update(app->input, dt);
		app->hud->tipex->Update(app->input, dt);
	}

	return true;
}

// Called each loop iteration
bool Hud::PostUpdate()
{
	bag->bounds.x = -app->render->camera.x + 1155;
	bag->bounds.y = -app->render->camera.y + 20;

	pencil->bounds.x = -app->render->camera.x + 204;
	pencil->bounds.y = -app->render->camera.y + 73;

	ball->bounds.x = -app->render->camera.x + 617;
	ball->bounds.y = -app->render->camera.y + 76;

	book->bounds.x = -app->render->camera.x + 441;
	book->bounds.y = -app->render->camera.y + 423;

	calculator->bounds.x = -app->render->camera.x + 424;
	calculator->bounds.y = -app->render->camera.y + 320;

	coffee->bounds.x = -app->render->camera.x + 418;
	coffee->bounds.y = -app->render->camera.y + 182;

	cola->bounds.x = -app->render->camera.x + 441;
	cola->bounds.y = -app->render->camera.y + 76;

	eraser->bounds.x = -app->render->camera.x + 204;
	eraser->bounds.y = -app->render->camera.y + 426;

	rule->bounds.x = -app->render->camera.x + 204;
	rule->bounds.y = -app->render->camera.y + 307;

	snack->bounds.x = -app->render->camera.x + 204;
	snack->bounds.y = -app->render->camera.y + 190;

	tipex->bounds.x = -app->render->camera.x + 629;
	tipex->bounds.y = -app->render->camera.y + 193;


	bool ret = true;

	
	if (!app->entityManager->settingsEnabled)
	{
		bag->Draw(app->render);
	}
	if (app->hud->bagEnabled && !app->entityManager->settingsEnabled)
	{
		app->render->DrawTexture(inventoryTab, -app->render->camera.x + 0, -app->render->camera.y + 0, NULL);
	}
	if (app->hud->bagEnabled && !app->entityManager->settingsEnabled)
	{
		pencil->Draw(app->render);
		ball->Draw(app->render);
		book->Draw(app->render);
		calculator->Draw(app->render);
		cola->Draw(app->render);
		coffee->Draw(app->render);
		eraser->Draw(app->render);
		rule->Draw(app->render);
		snack->Draw(app->render);
		tipex->Draw(app->render);
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
