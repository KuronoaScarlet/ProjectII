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
#include "SceneBath.h"

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


	//PAUSE.................................................................
	pauseMenu = app->tex->Load("Assets/Textures/Screens/pause_screen.png");


	settingsPost = app->tex->Load("Assets/Textures/postit.png");

	dUp = app->tex->Load("Assets/Textures/controller_1.png");
	dDown = app->tex->Load("Assets/Textures/controller_2.png");
	dLeft = app->tex->Load("Assets/Textures/controller_3.png");
	dRight = app->tex->Load("Assets/Textures/controller_4.png");

	music_s = app->tex->Load("Assets/Textures/fx_s.png");
	fx_s = app->tex->Load("Assets/Textures/music_s.png");

	resumeButton = new GuiButton(300, { 517,304, 240, 60 }, "CONTINUE");
	resumeButton->SetObserver(this);
	resumeButton->SetTexture(app->tex->Load("Assets/Textures/resume.png"), app->tex->Load("Assets/Textures/resume_selected.png"), app->tex->Load("Assets/Textures/resume_pressed.png"));

	settingsButton = new GuiButton(301, { 517, 409, 234, 55 }, "SETTINGS");
	settingsButton->SetObserver(this);
	settingsButton->SetTexture(app->tex->Load("Assets/Textures/settings.png"), app->tex->Load("Assets/Textures/settings_selected.png"), app->tex->Load("Assets/Textures/settings_pressed.png"));

	fullScreen = new GuiCheckBox(303, { 620,400, 300, 60 }, "FULLSCREEN");
	fullScreen->SetObserver(this);
	fullScreen->SetTexture(app->tex->Load("Assets/Textures/fs1.png"), app->tex->Load("Assets/Textures/fs2.png"), app->tex->Load("Assets/Textures/fs2.png"));
	
	vSync = new GuiCheckBox(306, { 620,400, 300, 60 }, "VSYNC");
	vSync->SetObserver(this);
	vSync->SetTexture(app->tex->Load("Assets/Textures/vs1.png"), app->tex->Load("Assets/Textures/vs2.png"), app->tex->Load("Assets/Textures/vs2.png"));

	musicSlider = new GuiSlider(304, { 1140,300, 60, 60 }, "FULLSCREEN");
	musicSlider->SetObserver(this);
	musicSlider->SetTexture(app->tex->Load("Assets/Textures/fx.png"), app->tex->Load("Assets/Textures/fx_selected.png"), app->tex->Load("Assets/Textures/fx_focused.png"));

	fxSlider = new GuiSlider(305, { 1140,350, 60, 60 }, "FULLSCREEN");
	fxSlider->SetObserver(this);
	fxSlider->SetTexture(app->tex->Load("Assets/Textures/fx.png"), app->tex->Load("Assets/Textures/fx_selected.png"), app->tex->Load("Assets/Textures/fx_focused.png"));

	exitButton = new GuiButton(302, { 551, 360, 172, 55 }, "CREDITS");
	exitButton->SetObserver(this);
	exitButton->SetTexture(app->tex->Load("Assets/Textures/exit.png"), app->tex->Load("Assets/Textures/exit_selected.png"), app->tex->Load("Assets/Textures/exit_pressed.png"));

	musicSliderBack = { 900,300,300,40 };
	fxSliderBack = { 900,350,300,40 };


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
	
	sharpened = new GuiButton(131, { 204,73, 69, 72 }, "PENCIL");// 1155,20
	sharpened->SetObserver(this);
	sharpened->SetTexture(app->tex->Load("Assets/Textures/Items/sharpened.png"), app->tex->Load("Assets/Textures/Items/sharpened_1.png"), app->tex->Load("Assets/Textures/Items/sharpened_2.png"));

	ball = new GuiButton(132, { 617,76, 70, 70 }, "PENCIL");// 1155,20
	ball->SetObserver(this);
	ball->SetTexture(app->tex->Load("Assets/Textures/Items/ball.png"), app->tex->Load("Assets/Textures/Items/ball_1.png"), app->tex->Load("Assets/Textures/Items/ball_2.png"));

	book = new GuiButton(133, { 441,423, 47, 70 }, "PENCIL");// 1155,20
	book->SetObserver(this);
	book->SetTexture(app->tex->Load("Assets/Textures/Items/book.png"), app->tex->Load("Assets/Textures/Items/book_1.png"), app->tex->Load("Assets/Textures/Items/book_2.png"));

	calculator = new GuiButton(134, { 424,320, 71, 69 }, "PENCIL");// 1155,20
	calculator->SetObserver(this);
	calculator->SetTexture(app->tex->Load("Assets/Textures/Items/calculator.png"), app->tex->Load("Assets/Textures/Items/calculator_1.png"), app->tex->Load("Assets/Textures/Items/calculator_2.png"));

	coffee = new GuiButton(135, { 418,182, 83, 80 }, "PENCIL");// 1155,20
	coffee->SetObserver(this);
	coffee->SetTexture(app->tex->Load("Assets/Textures/Items/coffe.png"), app->tex->Load("Assets/Textures/Items/coffe_1.png"), app->tex->Load("Assets/Textures/Items/coffe_2.png"));

	cola = new GuiButton(136, { 441,76, 37, 69 }, "PENCIL");// 1155,20
	cola->SetObserver(this);
	cola->SetTexture(app->tex->Load("Assets/Textures/Items/cola.png"), app->tex->Load("Assets/Textures/Items/cola_1.png"), app->tex->Load("Assets/Textures/Items/cola_2.png"));

	eraser = new GuiButton(137, { 204,426, 64, 70 }, "PENCIL");// 1155,20
	eraser->SetObserver(this);
	eraser->SetTexture(app->tex->Load("Assets/Textures/Items/eraser.png"), app->tex->Load("Assets/Textures/Items/eraser_1.png"), app->tex->Load("Assets/Textures/Items/eraser_2.png"));

	rule = new GuiButton(138, { 204,307, 74, 74 }, "PENCIL");// 1155,20
	rule->SetObserver(this);
	rule->SetTexture(app->tex->Load("Assets/Textures/Items/rule.png"), app->tex->Load("Assets/Textures/Items/rule_1.png"), app->tex->Load("Assets/Textures/Items/rule_2.png"));

	snack = new GuiButton(139, { 204,190, 67, 72 }, "PENCIL");// 1155,20
	snack->SetObserver(this);
	snack->SetTexture(app->tex->Load("Assets/Textures/Items/snack.png"), app->tex->Load("Assets/Textures/Items/snack_1.png"), app->tex->Load("Assets/Textures/Items/snack_2.png"));

	tipex = new GuiButton(140, { 629,193, 46, 80 }, "tipex");// 1155,20
	tipex->SetObserver(this);
	tipex->SetTexture(app->tex->Load("Assets/Textures/Items/tipex.png"), app->tex->Load("Assets/Textures/Items/tipex_1.png"), app->tex->Load("Assets/Textures/Items/tipex_2.png"));
	
	sharpenedPencil = new GuiButton(141, { 629,423, 46, 80 }, "sharpenedPencil");// 1155,20
	sharpenedPencil->SetObserver(this);
	sharpenedPencil->SetTexture(app->tex->Load("Assets/Textures/Items/pencil_sharpened.png"), app->tex->Load("Assets/Textures/Items/pencil_sharpened_1.png"), app->tex->Load("Assets/Textures/Items/pencil_sharpened_2.png"));
	
	Wonster = new GuiButton(142, { 629,423, 46, 80 }, "Monster sin copyright");// 1155,20
	Wonster->SetObserver(this);
	Wonster->SetTexture(app->tex->Load("Assets/Textures/Items/wonster.png"), app->tex->Load("Assets/Textures/Items/wonster_1.png"), app->tex->Load("Assets/Textures/Items/wonster_2.png"));

	inventoryTab = app->tex->Load("Assets/Textures/inventory_tab.png");
	inventory = app->tex->Load("Assets/Textures/Screens/inventory.png");

	statsTab = app->tex->Load("Assets/Textures/stats_tab.png");
	playerFace = app->tex->Load("Assets/Textures/player_face.png");
	allyFace = app->tex->Load("Assets/Textures/ally_face.png");

	holaRay = true;
	startglitch = true;

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
	GamePad& pad = app->input->pads[0];
	if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.up) && holaRay == true)
	{
		selectedId--;
		holaRay = false;
	}

	if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.down) && holaRay == true)
	{

		selectedId++;
		holaRay = false;
	
	}
	if (!pad.down && !pad.up)
	{
		holaRay = true;
	}
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

	if (!settingsEnabled)
	{
		musicSlider->bounds.x = -app->render->camera.x+1140;
		fxSlider->bounds.x = -app->render->camera.x+1140;
	}

	musicSlider->bounds.y = -app->render->camera.y + 220;
	fxSlider->bounds.y = -app->render->camera.y + 300;

	//RECT OF THE BACKGORUND ONLY UPDATE IF UPDATE THE SLIDERS.
	musicSliderBack.x = -app->render->camera.x + 935;
	musicSliderBack.y = -app->render->camera.y + 220;

	fxSliderBack.x = -app->render->camera.x + 935;
	fxSliderBack.y = -app->render->camera.y + 300;

	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		currentIteration = 0;
		bag->bounds.y = -app->render->camera.y;// +700;
		
		cantMoveInInvetory = !cantMoveInInvetory;
	}
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN || pad.up&&onGame)
	{
		bagEnabled = !bagEnabled;

	}
	if (app->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN || pad.down&&onGame)
	{
		statsEnabled = !statsEnabled;
	}
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || pad.left && onGame) app->sceneManager->showQuestMenu = !app->sceneManager->showQuestMenu;
	if(inventoryAndStatsRequest) pauseCondition = false;

	if (!settingsEnabled && inventoryAndStatsRequest)
	{
		bag->Update(app->input, dt);
		stats->Update(app->input, dt);
	}
	if (!settingsEnabled && bagEnabled)
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

	if (!settingsEnabled && statsEnabled)
	{
		quitStatsAndInvetory->Update(app->input, dt);
	}

	return true;
}

// Called each loop iteration
bool Hud::PostUpdate()
{
	GamePad& pad = app->input->pads[0];
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
	if ((app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || pad.right) && onGame)
	{
		app->hud->settingsEnabled = !app->hud->settingsEnabled;
	}

	if ((app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.start) && startglitch == true)
	{
		startglitch = false;
		pauseCondition = !pauseCondition;
		app->hud->settingsEnabled = false;
		app->hud->configOn = false;
		if(app->sceneManager->id == TITLE) selectedId = 501;
		else selectedId = 300;
	}
	if (!pad.start && !pad.up)
	{
		startglitch = true;
	}

	if (app->sceneManager->id == SCENE1 || app->sceneManager->id == SCENE12 || app->sceneManager->id == BATH || app->sceneManager->id == GYM)
	{
		app->render->DrawTexture(dUp, -app->render->camera.x + 990, -app->render->camera.y + 330, NULL);
		app->render->DrawTexture(dDown, -app->render->camera.x + 990, -app->render->camera.y + 500, NULL);
		app->render->DrawTexture(dLeft, -app->render->camera.x + 870, -app->render->camera.y + 465, NULL);
		app->render->DrawTexture(dRight, -app->render->camera.x + 1060, -app->render->camera.y + 465, NULL);
		onGame = true;
	}
	else
	{
		onGame = false;
	}

	if (bagEnabled && !settingsEnabled)
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

	if (statsEnabled && !settingsEnabled)
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
	settingsEnabled = false;
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

void Hud::DrawVictoryScreen()
{
	app->render->DrawRectangle({90, 40, 1100, 640 }, 0, 0, 0, 255);
	app->render->DrawRectangle({91, 41, 1098, 638 }, 255, 128, 128, 255);

	sprintf_s(text, 64, "Has ganado la pelea!");
	app->render->DrawText(app->render->font, text, 385, 90, 60, 4, { 0, 0, 0, 255 });
	sprintf_s(text, 64, "Buen trabajo! Sigue haciendolo asi!");
	app->render->DrawText(app->render->font, text, 360, 400, 40, 4, { 0, 0, 0, 255 });
	sprintf_s(text, 64, "Pulsa espacio para continuar...");
	app->render->DrawText(app->render->font, text, 770, 640, 35, 3, { 0, 0, 0, 255 });

	app->render->DrawTexture(playerFace, 470, 200, NULL);
	app->render->DrawTexture(allyFace, 670, 200, NULL);
}

void Hud::DrawLooseScreen()
{
	app->render->DrawRectangle({ 90, 40, 1100, 640 }, 0, 0, 0, 255);
	app->render->DrawRectangle({ 91, 41, 1098, 638 }, 128, 128, 255, 255);

	sprintf_s(text, 64, "Perdiste...");
	app->render->DrawText(app->render->font, text, 550, 90, 60, 4, { 0, 0, 0, 255 });
	sprintf_s(text, 64, "No te preocupes! Sigue intentandolo!");
	app->render->DrawText(app->render->font, text, 360, 400, 40, 4, { 0, 0, 0, 255 });
	sprintf_s(text, 64, "Pulsa espacio para continuar...");
	app->render->DrawText(app->render->font, text, 770, 640, 35, 3, { 0, 0, 0, 255 });

	app->render->DrawTexture(playerFace, 470, 200, NULL);
	app->render->DrawTexture(allyFace, 670, 200, NULL);
}