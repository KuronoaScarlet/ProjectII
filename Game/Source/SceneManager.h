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

	SceneManager(SceneType type);
	~SceneManager();

	bool Update(float dt);

	bool PostUpdate();

	void ChangeScene(SceneType type, float new_speed = 1000);

};

#endif // __SCENEMANAGER_H__