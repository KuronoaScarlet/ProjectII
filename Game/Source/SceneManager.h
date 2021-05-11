#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"

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

    bool Start(SceneType type);

	bool Update(float dt);

	bool PostUpdate();

	void ChangeScene(SceneType type, float new_speed = 1000);

    bool OnGuiMouseClickEvent(GuiControl* control);

    void OnMouseAboveButton(GuiControlState state, uint32 id);

    //PAUSE.........................................................
    GuiButton* resumeButton;
    GuiButton* settingsButton;
    GuiButton* exitButton;

    GuiCheckBox* fullScreen;

    SDL_Rect musicSliderBack;
    SDL_Rect fxSliderBack;

    SDL_Texture* settingsPost;
    SDL_Texture* pauseMenu = nullptr;

    bool pauseCondition = false;
    bool settingsEnabled = false;
    bool creditsOnScreen = false;
    bool creditSceneFlag;
    bool configOn = false;
    bool fullSc;
    bool vsync;
    bool exi;

    int volumMusic;
   
    //HUD.........................................................


};

#endif // __SCENEMANAGER_H__