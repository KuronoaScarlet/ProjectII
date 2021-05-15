#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "Easings.h"

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

struct Quest
{
    int id;
    const char* text;
    bool completed = false;
};

class SceneManager : public Module
{
public:

	Module* scene = nullptr;
	Module* next_scene = nullptr;

    enum FadeStep
    {
        NONE,
        WAIT1,
        TO_BLACK,
        WAIT2,
        FROM_BLACK
    } fadeStep = FadeStep::NONE;

    float alpha;
    float speed;

    SceneType id = LOGO;

    int transId = 0;

	SceneManager();
	~SceneManager();

    bool Start(SceneType type);

	bool Update(float dt);

	bool PostUpdate();

	void ChangeScene(SceneType type, int transId,float new_speed = 1000);

    bool OnGuiMouseClickEvent(GuiControl* control);

    void OnMouseAboveButton(GuiControlState state, uint32 id);

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
    bool scenegym = false;
    bool NPC1 = false;
    bool crate = false;
    int volumMusic;
   
    // 1st transition
    Easing* easing;
    SDL_Texture* trans1 = nullptr;
    int positionX = -1500;

    // 2n trasnition
    SDL_Texture* trans2 = nullptr;
    SDL_Texture* trans3 = nullptr;
    float positionY = -1040;
    float positionY2 = 1680;
    float positionY3 = 1680;
    float positionY4 = 1680;


    float currentIteration;
    float totalIterations;


    //BattleScene Bools.........................................................
    bool atkMenu = false;
    bool defMenu = false;
    bool combMenu = false;
    int itemSelection = 0;
    bool boost = false;

    //quest manager..................................................
    List<Quest*> quests;

    void CreateQuest(int _id, const char* _text);
    void CompleteQuest(int _id);
    void DisplayQuests();

    int newQuestAdded = 0;
    int currentIterationQuest = 0;

    SDL_Texture* back_quests;
    SDL_Texture* done_quests;
    SDL_Texture* todo_quests;
};

#endif // __SCENEMANAGER_H__