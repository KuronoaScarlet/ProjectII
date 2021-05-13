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

	Collider* godown1 = nullptr;
	Collider* goup = nullptr;
	Collider* goright = nullptr;
	Collider* goleft = nullptr;

	Collider* godown2 = nullptr;
	Collider* goup2 = nullptr;
	Collider* goright2 = nullptr;
	Collider* goleft2 = nullptr;

	Collider* godown3 = nullptr;
	Collider* goup3 = nullptr;
	Collider* goright3 = nullptr;
	Collider* goleft3 = nullptr;

	bool passingToLvl2 = false;
	bool lpl = false;


private:
	//GuiButton* play;
		
};


#endif // __SCENE1_H__