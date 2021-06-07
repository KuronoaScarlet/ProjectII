#include "GuiButton.h"
#include "SceneManager.h"
#include "HUD.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
    this->bounds = bounds;
    this->text = text;

    audio = false;
    audioB = false;
    audioR = false;
    selectFx = app->audio->LoadFx("Assets/Audio/Fx/menu_scroll.wav");
    selectedFx = app->audio->LoadFx("Assets/Audio/Fx/menu_selected.wav");
    releaseFx = app->audio->LoadFx("Assets/Audio/Fx/menu_release.wav");
    pencilFx = app->audio->LoadFx("Assets/Audio/Fx/pencil_circle.wav");

}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        // Check collision between mouse and button bounds
        if (app->hud->selectedId == id)
        {
            state = GuiControlState::FOCUSED;
            GamePad& pad = app->input->pads[0];
            if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_REPEAT || pad.a)
            {
                state = GuiControlState::PRESSED;
            }

            // If mouse button pressed -> Generate event!
            if (input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_UP || pad.a)
            {
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiButton::Draw(Render* render)
{
    // Draw the right button depending on state
    app->sceneManager->OnMouseAboveButton(state, id);
    if (!app->debugButton)
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
            render->DrawTexture(textureDisable, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::NORMAL:
            render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
            audio = false;
            audioB = false;
            break;
        case GuiControlState::FOCUSED:
            render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);
            if (audio == false)
            {
                audio = true;
                app->audio->PlayFx(channel,pencilFx);
            }
            break;
        case GuiControlState::PRESSED:
            render->DrawTexture(texturePressed, bounds.x, bounds.y, NULL);
           
            if (audioB == false)
            {
                audioB = true;
                app->audio->PlayFx(channel,selectedFx);
            }
          
            break;
        case GuiControlState::SELECTED:
            render->DrawRectangle(bounds, 0, 255, 0, 255);
            render->DrawTexture(texturePressed, bounds.x, bounds.y, NULL);
            if (audio == false)
            {
                audio = true;
                app->audio->PlayFx(channel,selectedFx);
            }
            break;
        default:
            break;
        }
    }
    else
    {
        switch (state)
        {
        case GuiControlState::DISABLED:
            render->DrawRectangle(bounds, 100, 100, 100, 255);
            break;
        case GuiControlState::NORMAL:
            render->DrawRectangle(bounds, 100, 100, 100, 255);
            audio = false;
            break;
        case GuiControlState::FOCUSED:
            render->DrawRectangle(bounds, 100, 25, 70, 255);
            if (audio == false)
            {
                audio = true;
                app->audio->PlayFx(channel, selectedFx);
            }
            break;
        case GuiControlState::PRESSED:
            render->DrawRectangle(bounds, 100, 100, 50, 255);
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
        default:
            break;
        }
    }

    return false;
}
