#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "BattleScene.h"
#include "EntityManager.h"
#include "Collisions.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "Title.h"
#include "DialogSystem.h"

#include "Defs.h"
#include "Log.h"

BattleScene::BattleScene() : Module()
{
	name.Create("battlescene");
}

// Destructor
BattleScene::~BattleScene()
{}

// Called before render is available
bool BattleScene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool BattleScene::Start()
{
	app->audio->PlayMusic("Assets/Audio/Music/battleSong.ogg");

	//BUTTONS---------------------------------------------------------
	attack = new GuiButton(1, { 517, 304, 250, 80 }, "attack");
	attack->SetObserver((Scene1*)this);
	attack->SetTexture(app->tex->Load("Assets/Textures/finger0.png"), app->tex->Load("Assets/Textures/finger1.png"), app->tex->Load("Assets/Textures/finger2.png"));

	defend = new GuiButton(1, { 800, 304, 250, 80 }, "defend");
	defend->SetObserver((Scene1*)this);
	defend->SetTexture(app->tex->Load("Assets/Textures/defend0.png"), app->tex->Load("Assets/Textures/defend1.png"), app->tex->Load("Assets/Textures/defend2.png"));

	run = new GuiButton(1, { 875, 400, 250, 80 }, "run");
	run->SetObserver((Scene1*)this);
	run->SetTexture(app->tex->Load("Assets/Textures/run0.png"), app->tex->Load("Assets/Textures/run1.png"), app->tex->Load("Assets/Textures/run2.png"));

	combine = new GuiButton(1, { 517, 400, 250, 80 }, "combine");
	combine->SetObserver((Scene1*)this);
	combine->SetTexture(app->tex->Load("Assets/Textures/combine0.png"), app->tex->Load("Assets/Textures/combine1.png"), app->tex->Load("Assets/Textures/combine2.png"));
	app->battleScene->active = true;
	//--------------------------------------------------------------
	

	app->render->camera = { 0, 0 };

	//Paso 0: Conseguir guardar la posición del player en una variable global y que tras terminar la pelea no comience en Spawn.
	//Paso 1: Cargar texturas, botones, música y Fx.
	//Paso 2: Añadir entidades enemigas. Random (2 o 3).
	//Paso 3: Añadir player y aliados (Animación de Idle lateral "onFight").
	//Paso 4: Cargar estadísticas de ambos bandos a partir de un xml. (Nivel de los enemigos a partir del player, multiplier desde la base).
	//Paso 5: Comenzar el ciclo de la barra de turno.

	app->entityManager->AddEntity({ 150.0f, 192.0f }, Entity::Type::ALLY1);
	app->entityManager->AddEntity({ 150.0f, 256.0f }, Entity::Type::PLAYER);


	return true;
}

// Called each loop iteration
bool BattleScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool BattleScene::Update(float dt)
{
	attack->Update(app->input, dt);
	run->Update(app->input, dt);
	defend->Update(app->input, dt);
	combine->Update(app->input, dt);
	//LÓGICA DEL BATTLE SYSTEM:
		//While que llama de manera permanente a las funciones de carga de la barra de turno. 
		//Break del while cuando salte turno. Tener un puntero que apunte a la unidad que le toca turno.

			//Si es el player o un aliado:
				//Blitear texto, botones, función recibe la acción en cuestión.
			//Si es enemigo:
				//A través de randoms que se pickee una acción.

		//Función para hacer perform de la acción (Texto + animación (si da tiempo))
		
		//De vuelta al while.

	return true;
}

// Called each loop iteration
bool BattleScene::PostUpdate()
{
	SDL_Rect bg{ 0,0,1280,720 };
	app->render->DrawRectangle(bg, 255, 212, 2, 255, true, false);

	attack->Draw(app->render);
	run->Draw(app->render);
	defend->Draw(app->render);
	combine->Draw(app->render);
	
	bool ret = true;
	return ret;
}

// Called before quitting
bool BattleScene::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();
	app->collisions->CleanUp();

	app->battleScene->active = false;

	LOG("Freeing scene");
	return true;
}
