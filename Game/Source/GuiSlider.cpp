#include "GuiSlider.h"
#include "SceneManager.h"
#include "Audio.h"
GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    sampleFx = app->audio->LoadFx("Assets/Audio/Fx/sample.wav");
    selectFx = app->audio->LoadFx("Assets/Audio/FX/menu_scroll.wav");
    selectedFx = app->audio->LoadFx("Assets/Audio/FX/menu_selected.wav");
    releaseFx = app->audio->LoadFx("Assets/Audio/FX/menu_release.wav");
    pencilFx = app->audio->LoadFx("Assets/Audio/FX/pencil_circle.wav");
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{

    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        int mouseMotionX, mouseMotionY;
        input->GetMouseMotion(mouseMotionX, mouseMotionY);

        mouseX -= app->render->camera.x;
        mouseY -= app->render->camera.y;

        SDL_Point mouse = { mouseX , mouseY };

        if (SDL_PointInRect(&mouse, &bounds))
        {
            state = GuiControlState::FOCUSED;
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;

                bounds.x = mouse.x - (bounds.w/2);
                int left = app->sceneManager->musicSliderBack.x + 1;
                if (bounds.x < left)
                    bounds.x = left;
                int right = app->sceneManager->musicSliderBack.x + app->sceneManager->musicSliderBack.w - 1 - bounds.w;
                if (bounds.x > right)
                    bounds.x = right;
                NotifyObserver();
            }
            onSquare = true;
        }
        else
        {
            onSquare = false;
        }
        if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
        {
            state = GuiControlState::RELEASED;
        }

           
        

        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::Draw(Render* render)
{
    if (!app->debugButton)
    {
        // Draw the right button depending on state
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
            break;
        case GuiControlState::NORMAL:
            render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
            audio = false;
            audioB = false;
            break;
        case GuiControlState::FOCUSED:
            render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::PRESSED:
        {
            render->DrawTexture(texturePressed, bounds.x, bounds.y, NULL);
            //app->audio->PlayFx(channel, sampleFx);
        }
            break;
        case GuiControlState::SELECTED:
        {
            render->DrawRectangle(bounds, 0, 255, 0, 255);
        }
            break;
        case GuiControlState::RELEASED:
        {
            render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
            if (audio == false && onSquare)
            {
                audio = true;
                app->audio->PlayFx(channel, sampleFx);
            }
        }
        default:
            break;
        }
    }
    else
    {
        switch (state)
        {
        case GuiControlState::DISABLED: render->DrawRectangle(bounds, 100, 100, 100, 255);
            break;
        case GuiControlState::NORMAL: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
        case GuiControlState::FOCUSED:
            render->DrawRectangle(bounds, 255, 255, 0, 255);
            if (audio == false)
            {
                audio = true;
                app->audio->PlayFx(channel, selectedFx);
            }
            break;
        case GuiControlState::PRESSED:
        {
            render->DrawRectangle(bounds, 0, 255, 255, 255);
            app->audio->PlayFx(channel, sampleFx);
        }
            break;
        case GuiControlState::SELECTED:
        {
            render->DrawRectangle(bounds, 0, 255, 0, 255);
           
        }
            break;
        default:
            break;
        }
    }

    return false;
}
