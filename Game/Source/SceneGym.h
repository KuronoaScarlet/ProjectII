#ifndef __SCENEGYM_H__
#define __SCENEGYM_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"
#include "Title.h"
class GuiCheckBox;
class GuiButton;
class GuiControl;
#include "GuiButton.h"
#include "GuiCheckBox.h"

class SceneGym : public Module
{
public:

	SceneGym();

	// Destructor
	virtual ~SceneGym();

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

	Collider* tp1to24 = nullptr;
	Collider* tp1to25 = nullptr;
	Collider* tp1to26 = nullptr;

	bool passingToLvl2 = false;
	bool lpl = false;


private:
	//GuiButton* play;
		
};


#endif // __SCENE1_H__