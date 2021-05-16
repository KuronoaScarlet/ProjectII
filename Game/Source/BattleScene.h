#ifndef __BATTLESCENE_H__
#define __BATTLESCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"
#include "Entity.h"
#include "Fonts.h"
#include "List.h"
#include "Title.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

class GuiControl;
class GuiSlider;

class BattleScene : public Module
{
public:
	BattleScene();

	// Destructor
	virtual ~BattleScene();

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

	enum TurnSort
	{
		PLAYER_TURN,
		ENEMY_TURN,

		UNKNOWN
	};

	enum CombatState
	{
		SELECT_ACTION,
		PERFORM_ACTION,
		FINISH_TURN,

		VICTORY,
		LOSE,

		WAITING
	};

	void PerformCombat(float dt);

	void ShowMenu(float dt);

	int SelectEnemy(int enemyNum);

	int SelectAlly(int allyNum);

	void SelectItem();

	void DealDamage(Entity* attacker, Entity* deffender);

	void Boost(Entity* attacker);

	void ResumeCombat();

	void PrintText();

	void PrintFace();



	bool OnGuiMouseClickEvent(GuiControl* control)
	{
		app->sceneManager->OnGuiMouseClickEvent(control);
		return true;
	}

public:

	SDL_Texture* screen = nullptr;
	SDL_Texture* combatBox = nullptr;
	SDL_Texture* winScreen = nullptr;

	SDL_Texture* playerFace = nullptr;
	SDL_Texture* allyFace = nullptr;
	SDL_Texture* dmgEnemyFace = nullptr;
	SDL_Texture* tankEnemyFace = nullptr;
	SDL_Texture* blncEnemyFace = nullptr;

	bool onTurn = false;
	Entity* turnEntity;
	ListItem<Entity*>* tmp;
	int enemySelection = 0;
	int allySelection = 0;
	bool itemSelected = false;
	bool boosted = false;
	bool victory = false;
	bool lose = false;

	TurnSort turn = UNKNOWN;
	CombatState state = WAITING;

	GuiButton* attack;
	GuiButton* defend;
	GuiButton* run;
	GuiButton* combine;

	int rngEnemyNum;
	int rngTypeEnemy;
	int remainingAllies, remainingEnemies;

	char hp[64] = { 0 };
	char num[64] = { 0 };
	char battleText[64] = { 0 };
	char turnText[64] = { 0 };

	uint hit = 0;
	uint powerUp = 0;

	SDL_Rect skipBarMax = { 0,0,0,0 };
	SDL_Rect skipBar = { 0,0,0,0 };
};


#endif // __BATTLESCENE_H__