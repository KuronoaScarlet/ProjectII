#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Entity.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "Title.h"
#include "SceneManager.h"

#include "PugiXml/src/pugixml.hpp"

class EntityManager : public Module
{
public:
	EntityManager();

	// Destructor
	virtual ~EntityManager()
	{}

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void AddEntity(fPoint position, Entity::Type type);

	void OnCollision(Collider* a, Collider* b);

	void LoadStats(Entity* e);

	struct PlayerData 
	{
		fPoint position;

		//Pause Menu
		bool debug = false;
		bool resetCamera;

		SDL_Rect cameraBckUp;

		//Dialog
		bool onDialog = false;

		//Combat Stats
		int level = 1;

		//Last Scene
		int scene = 0;

		//Items
		int pencil = 0;
		int sharper = 0;
		int ball = 0;
		int book = 0;
		int calculator = 0;
		int cola = 0;
		int coffee = 0;
		int fak = 0;//first aid kit
		int tipex = 0;
		int eraser = 0;
		int rule = 0;
		int celo = 0;
		int snack = 0;
		int pencilSharpened = 0;
		int wonster = 0;

		bool tp = false;
	};

	int winCount = 0;

	PlayerData playerData;

	List<Entity*> entityList;

	pugi::xml_document combatStats;


private:
	SDL_Texture* texPlayer;
	SDL_Texture* texAlly1;

	SDL_Texture* texNPC1;
	SDL_Texture* texNPC2;
	SDL_Texture* texNPC3;

	SDL_Texture* texEnemy1;
	SDL_Texture* texEnemy3;
	SDL_Texture* texEnemyLantern1;

	SDL_Texture* texItem;


	Entity* entityPlayer;
	Entity* entityAlly1;

	Entity* entityNPC1;
	Entity* entityNPC2;
	Entity* entityNPC3;
	Entity* entityCrate;

	Entity* entityGhostEnemy;
	Entity* entityTankEnemy;
	Entity* entityCanonEnemy;
	Entity* entityEnemyLantern1;
	Entity* entityEnemyLantern2;
	Entity* entityEnemyLantern3;

	Entity* entityPencil;
	Entity* entityBall;
	Entity* entityBook;
	Entity* entityCalculator;
	Entity* entityCan;
	Entity* entityCoffee;
	Entity* entityFistAidKit;
	Entity* entityLiquidPaper;
	Entity* entityPencilSharpener;
	Entity* entityRubber;
	Entity* entityRule;
	Entity* entityScotchTape;
	Entity* entitySnack;

	//Fonts
	int scoreFont = -1;
	char scoreText[10] = { "\0" };
};

#endif // _ENTITYMANAGER_H_