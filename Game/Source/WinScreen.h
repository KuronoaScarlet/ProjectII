#ifndef __WINSCREEN_H__
#define __WINSCREEN_H__

#include "Module.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class WinScreen : public Module
{
public:
    //Constructor
    WinScreen();

    //Destructor
    ~WinScreen();

    bool Awake();

    // Called when the module is activated
    // Loads the necessary textures for the map background
    bool Start();

    bool PreUpdate();

    Timer timer;
    int count = 6;
    int finalTimer = 0;
    // Called at the middle of the application loop
    // Updates the scene's background animations
    bool Update(float dt);

    // Called at the end of the application loop.
    // Performs the render call of all the parts of the scene's background
    bool PostUpdate();

    bool CleanUp();

public:
    // The scene sprite sheet loaded into an SDL_Texture
    SDL_Texture* screen = nullptr;

};

#endif