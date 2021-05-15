#ifndef __TITLE_H__
#define __TITLE_H__

#include "Module.h"
#include "Animation.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Easings.h"
#include "SceneManager.h"

class GuiControl;
class GuiSlider;

struct SDL_Texture;

class Title : public Module
{
public:
    //Constructor
    Title();

    //Destructor
    ~Title();

    bool Awake();

    // Called when the module is activated
    // Loads the necessary textures for the map background
    bool Start();

    bool PreUpdate();

    // Called at the middle of the application loop
    // Updates the scene's background animations
    bool Update(float dt);

    // Called at the end of the application loop.
    // Performs the render call of all the parts of the scene's background
    bool PostUpdate();

    bool CleanUp();

    bool OnGuiMouseClickEvent(GuiControl* control)
    {
        app->sceneManager->OnGuiMouseClickEvent(control);
        return true;
    }



public:
    Easing* easing;
    // The scene sprite sheet loaded into an SDL_Texture
    SDL_Texture* screen = nullptr;
    SDL_Texture* bck = nullptr;
    SDL_Texture* caronte = nullptr;
    SDL_Texture* mandate = nullptr;
    ////////Easings
    float positionTitleBack;
    float positionTitleCaronte;
    float positionTitleMandate;
    float currentIterationBck;
    float currentIterationCaronte;
    float currentIterationMandate;
    float totalIterations;
    bool startCaronte = true;

    SDL_Texture* creditsScene = nullptr;

    
    SDL_Texture* settingsPost2;
    GuiButton* play;
    GuiButton* newGame;
    GuiButton* escCredits;
    GuiButton* options;
    GuiButton* credits;
    GuiButton* exit;
    GuiCheckBox* fullScreen;
    GuiSlider* musicVolume;
    GuiSlider* fxVolume;
    
    bool pauseBool;

    char text[64] = { 0 };
};

#endif