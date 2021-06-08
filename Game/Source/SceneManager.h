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

   
    bool scenebath = false;
    bool scenegym = false;
    bool checkpointgym = false;
    bool NPC1 = false;
    bool crate = false;
   
   
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

    //Puzzle
    bool blueballbutton = false;
    bool redballbutton = false;
    bool purpleballbutton = false;
    bool yellowballbutton = false;


    //BattleScene Bools and Variables.........................................................
    bool atkMenu = false;
    bool defMenu = false;
    bool combMenu = false;
    int enemySelection = 0;
    bool enemySelected = false;
    int itemSelection = 0;
    bool boost = false;

    //quest manager..................................................
    List<Quest*> quests;

    void CreateQuest(int _id, const char* _text);
    void CompleteQuest(int _id);
    void DisplayQuests();

    int newQuestAdded = 0;
    int currentIterationQuest = 0;
    bool showQuestMenu = false;

    SDL_Texture* back_quests;
    SDL_Texture* done_quests;
    SDL_Texture* todo_quests;
};

#endif // __SCENEMANAGER_H__