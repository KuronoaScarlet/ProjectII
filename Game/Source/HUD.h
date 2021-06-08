#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"
#include "Title.h"
class GuiCheckBox;
class GuiButton;
class GuiControl;
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "Entity.h"
#include "EntityManager.h"

class Hud : public Module
{
public:

	Hud();

	// Destructor
	virtual ~Hud();

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

	bool OnGuiMouseClickEvent(GuiControl* control)
	{
		app->sceneManager->OnGuiMouseClickEvent(control);
		return true;
	}

	void InventoryAndStatsRequest();

	void DrawStatsText(Entity* e);

	void DrawVictoryScreen();
	void DrawLooseScreen();

	int selectedId = 501;
	bool holaRay = true;
	bool startglitch = true;
	bool optionsGlitch = true;
	bool questGlitch = true;
	//PAUSE.........................................................
	GuiButton* resumeButton;
	GuiButton* settingsButton;
	GuiButton* exitButton;

	GuiSlider* musicSlider;
	GuiSlider* fxSlider;

	GuiCheckBox* fullScreen;
	GuiCheckBox* vSync;

	SDL_Texture* fx_s = nullptr;
	SDL_Texture* music_s = nullptr;

	SDL_Rect musicSliderBack;
	SDL_Rect fxSliderBack;

	SDL_Texture* settingsPost = nullptr;
	SDL_Texture* pauseMenu = nullptr;

	bool pauseCondition = false;
	bool settingsEnabled = false;
	bool creditsOnScreen = false;
	bool creditSceneFlag = false;
	bool configOn = false;
	bool fullSc = false;
	bool vsync = false;
	bool exi = false;
	int volumMusic;

	bool firstEntry = true;
	bool on = false;

	SDL_Texture* inventoryTab = nullptr;
	SDL_Texture* statsTab = nullptr;
	SDL_Texture* inventory = nullptr;

	SDL_Texture* padControlls = nullptr;

	bool onGame = false;

	SDL_Texture* playerFace = nullptr;
	SDL_Texture* allyFace = nullptr;

	char statsText[64] = { 0 };
	char text[64] = { 0 };

	bool bagEnabled = false;
	bool pencilEnabled = false;
	bool ballEnabled = false;
	bool bookEnabled = false;
	bool calculatorEnabled = false;
	bool coffeeEnabled = false;
	bool colaEnabled = false;
	bool eraserEnabled = false;
	bool ruleEnabled = false;
	bool tipexEnabled = false;
	bool snackEnabled = false;
	bool sharpenedEnabled = false;

	bool statsEnabled = false;

	bool inventoryAndStatsRequest = false;
	bool cantMoveInInvetory = false;

	GuiButton* bag;
	int bckposY = 0;
	GuiButton* stats;
	GuiButton* quitStatsAndInvetory;

	GuiButton* pencil;
	GuiButton* sharpened;
	GuiButton* ball;
	GuiButton* book;
	GuiButton* calculator;
	GuiButton* coffee;
	GuiButton* cola;
	GuiButton* eraser;
	GuiButton* rule;
	GuiButton* tipex;
	GuiButton* snack;
	GuiButton* sharpenedPencil;
	GuiButton* Wonster;
private:
	//GuiButton* play;

	Easing* easing;
	
	float currentIteration = 0;
	float totalIterations = 0;
	float initialPosition = 0;
	float deltaPosition = 0;

};


#endif // __HUD_H__