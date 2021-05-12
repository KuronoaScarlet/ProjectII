#ifndef __SCENEBATH_H__
#define __SCENEBATH_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"
#include "Title.h"
class GuiCheckBox;
class GuiButton;
class GuiControl;
#include "GuiButton.h"
#include "GuiCheckBox.h"

class SceneBath : public Module
{
public:

	SceneBath();

	// Destructor
	virtual ~SceneBath();

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
		app->title->OnGuiMouseClickEvent(control);
		return true;
	}

	bool firstEntry = true;
	bool on;

	Collider* tp1to21 = nullptr;
	Collider* tp1to22 = nullptr;
	Collider* tp1to23 = nullptr;

	Collider* godown1 = nullptr;
	Collider* goup = nullptr;
	Collider* goright = nullptr;
	Collider* goleft = nullptr;

	Collider* godown2 = nullptr;
	Collider* goup2 = nullptr;
	Collider* goright2 = nullptr;
	Collider* goleft2 = nullptr;

	Collider* tp1to24 = nullptr;
	Collider* tp1to25 = nullptr;
	Collider* tp1to26 = nullptr;

	bool passingToLvl2 = false;
	bool lpl = false;


private:
	//GuiButton* play;
		
};


#endif // __SCENEBATH_H__