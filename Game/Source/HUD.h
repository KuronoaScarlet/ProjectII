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
		app->title->OnGuiMouseClickEvent(control);
		return true;
	}

	bool firstEntry = true;
	bool on;

	SDL_Texture* inventoryTab = nullptr;

	bool bagEnabled = false;

	GuiButton* bag;

	GuiButton* pencil;
private:
	//GuiButton* play;

};


#endif // __HUD_H__