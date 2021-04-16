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
	app->battleScene->active = true;

	//Paso 0: Conseguir guardar la posición del player en una variable global y que tras terminar la pelea no comience en Spawn.
	//Paso 1: Cargar texturas, botones, música y Fx.
	//Paso 2: Añadir entidades enemigas. Random (2 o 3).
	//Paso 3: Añadir player y aliados (Animación de Idle lateral "onFight").
	//Paso 4: Cargar estadísticas de ambos bandos a partir de un xml. (Nivel de los enemigos a partir del player, multiplier desde la base).
	//Paso 5: Comenzar el ciclo de la barra de turno.

	app->entityManager->AddEntity({ 100.0f, 175.0f }, Entity::Type::PLAYER);

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


	bool ret = true;
	return ret;
}

// Called before quitting
bool BattleScene::CleanUp()
{
	if (!active)return true;

	app->entityManager->CleanUp();

	app->battleScene->active = false;

	LOG("Freeing scene");
	return true;
}
