#ifndef __BATTLESCENE_H__
#define __BATTLESCENE_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"

class GuiControl;

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

	bool OnGuiMouseClickEvent(GuiControl* control);


	//Fonts
	Fonts* font;

	//int scoreFont = -1;
	char scoreText[64] = { 0 };
	char timeText[64] = { 0 };

	bool firstEntry = true;
	bool on;

	Timer timerr;
	int time = 0;

private:
};


#endif // __BATTLESCENE_H__