#ifndef __SCENE12_H__
#define __SCENE12_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"
#include "Title.h"
class GuiCheckBox;
class GuiButton;
class GuiControl;
#include "GuiButton.h"
#include "GuiCheckBox.h"

class Scene12 : public Module
{
public:

	Scene12();

	// Destructor
	virtual ~Scene12();

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
	bool passingToLvl1 = false;
	bool firstEntry = true;
	bool on;
	bool lpl = false;

	Collider* tp2to11 = nullptr;
	Collider* tp2to12 = nullptr;
	Collider* tp2to13 = nullptr;

	Collider* tp2to14 = nullptr;
	Collider* tp2to15 = nullptr;
	Collider* tp2to16 = nullptr;
private:
	//GuiButton* play;
		
};


#endif // __SCENE1_H__