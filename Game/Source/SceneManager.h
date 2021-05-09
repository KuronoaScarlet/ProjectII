#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"

enum SceneType
{
    LOGO,
    TITLE,
    INTRO,
    SCENE1,
    SCENE12,
    BATH,
    GYM,
    WIN,

    BATTLE,
};

class SceneManager : public Module
{
public:

	Module* scene = nullptr;
	Module* next_scene = nullptr;

    enum FadeStep
    {
        NONE,
        TO_BLACK,
        WAIT,
        FROM_BLACK
    } fadeStep = FadeStep::NONE;

    float alpha;
    float speed;

    SceneType id = LOGO;

	SceneManager();
	~SceneManager();

	bool Update(float dt);

	bool PostUpdate();

	void ChangeScene(SceneType type, float new_speed = 1000);

    bool OnGuiMouseClickEvent(GuiControl* control)
    {
        if(scene) scene->OnGuiMouseClickEvent(control);
        return true;
    }

    //PAUSE.........................................................
    GuiButton* resumeButton;
    GuiButton* settingsButton;
    GuiButton* exitButton;

    GuiCheckBox* fullScreen;

    SDL_Texture* settingsPost;
    SDL_Texture* pauseMenu = nullptr;

    bool pauseCondition = false;
    bool settingsEnabled = false;
   
    //HUD.........................................................


};

#endif // __SCENEMANAGER_H__