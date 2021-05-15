#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene1.h"
#include "Map.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Title.h"
#include "HUD.h"
#include "DialogSystem.h"
#include "SceneManager.h"

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
	currentIteration = 0;
	totalIterations = 20;
	initialPosition = 700;
	deltaPosition = -148;
	active = true;
	app->hud->active = true;
	bag = new GuiButton(120, { 275,(int)initialPosition, 90, 125 }, "BAG");// 1155,20
	bag->SetObserver(this);
	bag->SetTexture(app->tex->Load("Assets/Textures/bag.png"), app->tex->Load("Assets/Textures/bag2.png"), app->tex->Load("Assets/Textures/bag2.png"));

	stats = new GuiButton(121, { 947,556, 90, 125 }, "BAG");// 1155,20
	stats->SetObserver(this);
	stats->SetTexture(app->tex->Load("Assets/Textures/face.png"), app->tex->Load("Assets/Textures/face2.png"), app->tex->Load("Assets/Textures/face2.png"));

	quitStatsAndInvetory = new GuiButton(122, { 70,39, 86, 98 }, "BAG");// 1155,20
	quitStatsAndInvetory->SetObserver(this);
	quitStatsAndInvetory->SetTexture(app->tex->Load("Assets/Textures/esc4.png"), app->tex->Load("Assets/Textures/esc5.png"), app->tex->Load("Assets/Textures/esc6.png"));

	pencil = new GuiButton(130, { 204,73, 69, 72 }, "PENCIL");// 1155,20
	pencil->SetObserver(this);
	pencil->SetTexture(app->tex->Load("Assets/Textures/Items/Pencil.png"), app->tex->Load("Assets/Textures/Items/Pencil_1.png"), app->tex->Load("Assets/Textures/Items/Pencil_2.png"));
	
	sharpened = new GuiButton(130, { 204,73, 69, 72 }, "PENCIL");// 1155,20
	sharpened->SetObserver(this);
	sharpened->SetTexture(app->tex->Load("Assets/Textures/Items/sharpened.png"), app->tex->Load("Assets/Textures/Items/sharpened_1.png"), app->tex->Load("Assets/Textures/Items/sharpened_2.png"));

	ball = new GuiButton(131, { 617,76, 70, 70 }, "PENCIL");// 1155,20
	ball->SetObserver(this);
	ball->SetTexture(app->tex->Load("Assets/Textures/Items/ball.png"), app->tex->Load("Assets/Textures/Items/ball_1.png"), app->tex->Load("Assets/Textures/Items/ball_2.png"));

	book = new GuiButton(132, { 441,423, 47, 70 }, "PENCIL");// 1155,20
	book->SetObserver(this);
	book->SetTexture(app->tex->Load("Assets/Textures/Items/book.png"), app->tex->Load("Assets/Textures/Items/book_1.png"), app->tex->Load("Assets/Textures/Items/book_2.png"));

	calculator = new GuiButton(133, { 424,320, 71, 69 }, "PENCIL");// 1155,20
	calculator->SetObserver(this);
	calculator->SetTexture(app->tex->Load("Assets/Textures/Items/calculator.png"), app->tex->Load("Assets/Textures/Items/calculator_1.png"), app->tex->Load("Assets/Textures/Items/calculator_2.png"));

	coffee = new GuiButton(134, { 418,182, 83, 80 }, "PENCIL");// 1155,20
	coffee->SetObserver(this);
	coffee->SetTexture(app->tex->Load("Assets/Textures/Items/coffe.png"), app->tex->Load("Assets/Textures/Items/coffe_1.png"), app->tex->Load("Assets/Textures/Items/coffe_2.png"));

	cola = new GuiButton(135, { 441,76, 37, 69 }, "PENCIL");// 1155,20
	cola->SetObserver(this);
	cola->SetTexture(app->tex->Load("Assets/Textures/Items/cola.png"), app->tex->Load("Assets/Textures/Items/cola_1.png"), app->tex->Load("Assets/Textures/Items/cola_2.png"));

	eraser = new GuiButton(136, { 204,426, 64, 70 }, "PENCIL");// 1155,20
	eraser->SetObserver(this);
	eraser->SetTexture(app->tex->Load("Assets/Textures/Items/eraser.png"), app->tex->Load("Assets/Textures/Items/eraser_1.png"), app->tex->Load("Assets/Textures/Items/eraser_2.png"));

	rule = new GuiButton(137, { 204,307, 74, 74 }, "PENCIL");// 1155,20
	rule->SetObserver(this);
	rule->SetTexture(app->tex->Load("Assets/Textures/Items/rule.png"), app->tex->Load("Assets/Textures/Items/rule_1.png"), app->tex->Load("Assets/Textures/Items/rule_2.png"));

	snack = new GuiButton(138, { 204,190, 67, 72 }, "PENCIL");// 1155,20
	snack->SetObserver(this);
	snack->SetTexture(app->tex->Load("Assets/Textures/Items/snack.png"), app->tex->Load("Assets/Textures/Items/snack_1.png"), app->tex->Load("Assets/Textures/Items/snack_2.png"));

	tipex = new GuiButton(139, { 629,193, 46, 80 }, "tipex");// 1155,20
	tipex->SetObserver(this);
	tipex->SetTexture(app->tex->Load("Assets/Textures/Items/tipex.png"), app->tex->Load("Assets/Textures/Items/tipex_1.png"), app->tex->Load("Assets/Textures/Items/tipex_2.png"));
	
	sharpenedPencil = new GuiButton(140, { 629,423, 46, 80 }, "sharpenedPencil");// 1155,20
	sharpenedPencil->SetObserver(this);
	sharpenedPencil->SetTexture(app->tex->Load("Assets/Textures/Items/pencil_sharpened.png"), app->tex->Load("Assets/Textures/Items/pencil_sharpened_1.png"), app->tex->Load("Assets/Textures/Items/pencil_sharpened_2.png"));
	
	Wonster = new GuiButton(141, { 629,423, 46, 80 }, "Monster sin copyright");// 1155,20
	Wonster->SetObserver(this);
	Wonster->SetTexture(app->tex->Load("Assets/Textures/Items/wonster.png"), app->tex->Load("Assets/Textures/Items/wonster_1.png"), app->tex->Load("Assets/Textures/Items/wonster_2.png"));


	inventoryTab = app->tex->Load("Assets/Textures/inventory_tab.png");
	inventory = app->tex->Load("Assets/Textures/Screens/inventory.png");

	statsTab = app->tex->Load("Assets/Textures/stats_tab.png");
	playerFace = app->tex->Load("Assets/Textures/player_face.png");
	allyFace = app->tex->Load("Assets/Textures/ally_face.png");

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
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		currentIteration = 0;
		bag->bounds.y = -app->render->camera.y;// +700;
		inventoryAndStatsRequest = !inventoryAndStatsRequest;
		cantMoveInInvetory = !cantMoveInInvetory;
	}
	if(inventoryAndStatsRequest)
		app->sceneManager->pauseCondition = false;

	if (!app->sceneManager->settingsEnabled && inventoryAndStatsRequest)
	{
		bag->Update(app->input, dt);
		stats->Update(app->input, dt);
	}
	if (!app->sceneManager->settingsEnabled && bagEnabled)
	{
		pencil->Update(app->input, dt);
		sharpened->Update(app->input, dt);
		ball->Update(app->input, dt);
		calculator->Update(app->input, dt);
		book->Update(app->input, dt);
		coffee->Update(app->input, dt);
		cola->Update(app->input, dt);
		eraser->Update(app->input, dt);
		rule->Update(app->input, dt);
		snack->Update(app->input, dt);
		tipex->Update(app->input, dt);
		sharpenedPencil->Update(app->input, dt);
		Wonster->Update(app->input, dt);
		quitStatsAndInvetory->Update(app->input, dt);
	}

	if (!app->sceneManager->settingsEnabled && statsEnabled)
	{
		quitStatsAndInvetory->Update(app->input, dt);
	}


	return true;
}

// Called each loop iteration
bool Hud::PostUpdate()
{

	bag->bounds.x = -app->render->camera.x + 275;
	bag->bounds.y = easing->linearEaseInOut(currentIteration, -app->render->camera.y + 700, deltaPosition, totalIterations);
	bckposY = easing->linearEaseInOut(currentIteration, -app->render->camera.y + 700, deltaPosition, totalIterations);
	
	stats->bounds.x = -app->render->camera.x + 947;
	stats->bounds.y = easing->linearEaseInOut(currentIteration, -app->render->camera.y + 700, deltaPosition, totalIterations);;

	if (currentIteration < totalIterations)
	{
		++currentIteration;
	}

	quitStatsAndInvetory->bounds.x = -app->render->camera.x + 70;
	quitStatsAndInvetory->bounds.y = -app->render->camera.y + 39;

	if (inventoryAndStatsRequest)
	{
		InventoryAndStatsRequest();
	}

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
	
	sharpened->bounds.x = -app->render->camera.x + 620;
	sharpened->bounds.y = -app->render->camera.y + 310;

	sharpenedPencil->bounds.x = -app->render->camera.x + 629;
	sharpenedPencil->bounds.y = -app->render->camera.y + 400;

	Wonster->bounds.x = -app->render->camera.x + 810;
	Wonster->bounds.y = -app->render->camera.y + 73;

	bool ret = true;

	if (bagEnabled && !app->sceneManager->settingsEnabled)
	{
		app->render->DrawTexture(inventoryTab, -app->render->camera.x + 0, -app->render->camera.y + 0, NULL);
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
		sharpenedPencil->Draw(app->render);
		sharpened->Draw(app->render);
		Wonster->Draw(app->render);
		quitStatsAndInvetory->Draw(app->render);

		char pencilCount[80] = { 0 };
		sprintf_s(pencilCount, 80, "x%d", app->entityManager->playerData.pencil);
		app->render->DrawText(app->render->font, pencilCount, 300, 70, 60, 0, { 0, 0, 0, 255 });

		char ballCount[80] = { 0 };
		sprintf_s(ballCount, 80, "x%d", app->entityManager->playerData.ball);
		app->render->DrawText(app->render->font, ballCount, 700, 70, 60, 0, { 0, 0, 0, 255 });

		char bookCount[80] = { 0 };
		sprintf_s(bookCount, 80, "x%d", app->entityManager->playerData.book);
		app->render->DrawText(app->render->font, bookCount, 500, 430, 60, 0, { 0, 0, 0, 255 });

		char calculatorCount[80] = { 0 };
		sprintf_s(calculatorCount, 80, "x%d", app->entityManager->playerData.calculator);
		app->render->DrawText(app->render->font, calculatorCount, 500, 330, 60, 0, { 0, 0, 0, 255 });

		char colaCount[80] = { 0 };
		sprintf_s(colaCount, 80, "x%d", app->entityManager->playerData.cola);
		app->render->DrawText(app->render->font, colaCount, 500, 70, 60, 0, { 0, 0, 0, 255 });

		/*char fakCount[80] = { 0 };
		sprintf_s(fakCount, 80, "x%d", app->entityManager->playerData.fak);
		app->render->DrawText(app->render->font, fakCount, 300, 70, 60, 0, { 0, 0, 0, 255 });*/


		char coffeeCount[80] = { 0 };
		sprintf_s(coffeeCount, 80, "x%d", app->entityManager->playerData.coffee);
		app->render->DrawText(app->render->font, coffeeCount, 500, 200, 60, 0, { 0, 0, 0, 255 });


		char tipexCount[80] = { 0 };
		sprintf_s(tipexCount, 80, "x%d", app->entityManager->playerData.tipex);
		app->render->DrawText(app->render->font, tipexCount, 700, 200, 60, 0, { 0, 0, 0, 255 });


		char eraserCount[80] = { 0 };
		sprintf_s(eraserCount, 80, "x%d", app->entityManager->playerData.eraser);
		app->render->DrawText(app->render->font, eraserCount, 300, 430, 60, 0, { 0, 0, 0, 255 });


		char ruleCount[80] = { 0 };
		sprintf_s(ruleCount, 80, "x%d", app->entityManager->playerData.rule);
		app->render->DrawText(app->render->font, ruleCount, 300, 330, 60, 0, { 0, 0, 0, 255 });

		/*char celoCount[80] = { 0 };
		sprintf_s(celoCount, 80, "x%d", app->entityManager->playerData.celo);
		app->render->DrawText(app->render->font, celoCount, 300, 70, 60, 0, { 0, 0, 0, 255 });*/

		char snackCount[80] = { 0 };
		sprintf_s(snackCount, 80, "x%d", app->entityManager->playerData.snack);
		app->render->DrawText(app->render->font, snackCount, 300, 200, 60, 0, { 0, 0, 0, 255 });

		char sharpenerCount[80] = { 0 };
		sprintf_s(sharpenerCount, 80, "x%d", app->entityManager->playerData.sharper);
		app->render->DrawText(app->render->font, sharpenerCount, 700, 310, 60, 0, { 0, 0, 0, 255 });

		char pencilShapedCount[80] = { 0 };
		sprintf_s(pencilShapedCount, 80, "x%d", app->entityManager->playerData.pencilSharpened);
		app->render->DrawText(app->render->font, pencilShapedCount, 700, 418, 60, 0, { 0, 0, 0, 255 });

		char wonsterCount[80] = { 0 };
		sprintf_s(wonsterCount, 80, "x%d", app->entityManager->playerData.wonster);
		app->render->DrawText(app->render->font, wonsterCount, 880, 73, 60, 0, { 0, 0, 0, 255 });
	}

	if (statsEnabled && !app->sceneManager->settingsEnabled)
	{
		app->render->DrawTexture(statsTab, -app->render->camera.x + 0, -app->render->camera.y + 0, NULL);
		app->render->DrawTexture(playerFace, -app->render->camera.x + 270, -app->render->camera.y + 70, NULL);
		app->render->DrawRectangle({ -app->render->camera.x + 634, -app->render->camera.y + 60, 1, 600 }, 0, 0, 0, 255);
		app->render->DrawTexture(allyFace, -app->render->camera.x + 870, -app->render->camera.y + 70, NULL);

		app->entityManager->AddEntity({ 5000.0f, 5000.0f }, Entity::Type::ALLY1);

		DrawStatsText(app->entityManager->entityList.start->data);
		DrawStatsText(app->entityManager->entityList.end->data);

		quitStatsAndInvetory->Draw(app->render);
	}

	return ret;
}

// Called before quitting
bool Hud::CleanUp()
{
	app->hud->active = false;
	if (!active)return true;
	bag->CleanUp();

	pencil->CleanUp();
	sharpened->CleanUp();
	ball->CleanUp();
	book->CleanUp();
	calculator->CleanUp();
	coffee->CleanUp();
	cola->CleanUp();
	eraser->CleanUp();
	rule->CleanUp();
	tipex->CleanUp();
	snack->CleanUp();
	app->entityManager->CleanUp();

	LOG("Freeing scene");
	return true;
}

void Hud::InventoryAndStatsRequest()
{
	app->render->DrawTexture(inventory,-app->render->camera.x, bckposY);
	bag->Draw(app->render);
	stats->Draw(app->render);
	inventoryAndStatsRequest = true;
}

void Hud::DrawStatsText(Entity* e)
{
	int posRect = 0;
	int posText = 0;

	if (e->type == Entity::Type::PLAYER)
	{
		posRect = -app->render->camera.x + 190;
		posText = 190;
		sprintf_s(statsText, 64, "Player");
		app->render->DrawText(app->render->font, statsText, posText + 100, 198, 36, 3, { 0, 0, 0, 255 });
	}
	else if (e->type == Entity::Type::ALLY1)
	{
		posRect = -app->render->camera.x + 790;
		posText = 790;
		sprintf_s(statsText, 64, "Ally 1");
		app->render->DrawText(app->render->font, statsText, posText + 110, 198, 36, 3, { 0, 0, 0, 255 });
	}

	sprintf_s(statsText, 64, "HP: %d", e->hp);
	app->render->DrawText(app->render->font, statsText, posText, 258, 36, 3, { 0, 0, 0, 255 });
	app->render->DrawRectangle({ posRect+19, -app->render->camera.y + 309, ((e->hp)*3)+2, 20 }, 0, 0, 0, 255);
	app->render->DrawRectangle({ posRect+20, -app->render->camera.y + 310, (e->hp)*3, 18 }, 0, 255, 0, 255);

	sprintf_s(statsText, 64, "ATK: %d", e->atk);
	app->render->DrawText(app->render->font, statsText, posText, 358, 36, 3, { 0, 0, 0, 255 });
	app->render->DrawRectangle({ posRect+19, -app->render->camera.y + 409, ((e->atk)*3)+2, 20 }, 0, 0, 0, 255);
	app->render->DrawRectangle({ posRect+20, -app->render->camera.y + 410, (e->atk)*3, 18 }, 255, 0, 0, 255);

	sprintf_s(statsText, 64, "DEF: %d", e->def);
	app->render->DrawText(app->render->font, statsText, posText, 458, 36, 3, { 0, 0, 0, 255 });
	app->render->DrawRectangle({ posRect+19, -app->render->camera.y + 509, ((e->def)*3)+2, 20 }, 0, 0, 0, 255);
	app->render->DrawRectangle({ posRect+20, -app->render->camera.y + 510, (e->def)*3, 18 }, 0, 0, 255, 255);
}