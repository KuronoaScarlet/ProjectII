#ifndef _ENTITYMANAGER_H_
#define _ENTITYMANAGER_H_

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Entity.h"

#include "GuiButton.h"

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

	struct PlayerData 
	{
		fPoint position;
		int lives = 3;

		bool hit = false;
		bool footJump = false;

		fPoint initPositionScene1 = { 50.0f, 260.0f };
		fPoint initPositionScene2 = { 50.0f, 278.0f };
		fPoint initPositionScene3 = { 32.0f, 277.0f };
		fPoint initPositionScene4 = { 32.0f, 277.0f };

		//Pause Menu
		bool pauseCondition = false;
		bool debug = false;
		bool resetCamera;

		SDL_Texture* pauseMenu = nullptr;

		GuiButton* resumeButton;
		GuiButton* settingsButton;
		GuiButton* backToTitleButton;
		GuiButton* exitButton;

		SDL_Rect cameraBckUp;
	};

	PlayerData playerData;

	List<Entity*> entityList;
private:
	SDL_Texture* texPlayer;
	SDL_Texture* texNPC1;
	SDL_Texture* texNPC2;
	SDL_Texture* texNPC3;
	SDL_Texture* texEnemy1;
	SDL_Texture* texEnemy3;

	Entity* entityPlayer;
	Entity* entityNPC1;
	Entity* entityNPC2;
	Entity* entityNPC3;
	Entity* entityGhostEnemy;
	Entity* entityTankEnemy;
	Entity* entityCanonEnemy;

	//Fonts
	int scoreFont = -1;
	char scoreText[10] = { "\0" };
};

#endif // _ENTITYMANAGER_H_