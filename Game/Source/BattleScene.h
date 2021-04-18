#ifndef __BATTLESCENE_H__
#define __BATTLESCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"
#include "Entity.h"
#include "Fonts.h"
#include "List.h"

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

	void ResumeCombat();

	void DealDamage(Entity* attacker, Entity* deffender);

	void PrintText();

	bool OnGuiMouseClickEvent(GuiControl* control);

	//Fonts
	Fonts* font;

	SDL_Texture* screen = nullptr;
	SDL_Texture* combatBox = nullptr;

	//int scoreFont = -1

	bool firstEntry = true;
	bool onTurn = false;
	bool playerTurn = false;
	bool allyTurn = false;
	bool attackMenu = false;
	bool endTurn = false;
	bool attacked = false;
	bool win = false;
	bool loose = false;

	GuiButton* attack;
	GuiButton* defend;
	GuiButton* run;
	GuiButton* combine;

	Entity* pointer;
	Timer timerr;
	int time = 0;


	int rngEnemyNum;
	int rngTypeEnemy;
	int remainingAllies, remainingEnemies;
	int counter;
	
	char hp[64] = { 0 };
	char num[64] = { 0 };
	char battleText[64] = { 0 };
private:
};


#endif // __BATTLESCENE_H__