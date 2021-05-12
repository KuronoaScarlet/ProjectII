#include "GuiSlider.h"
#include "SceneManager.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
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
        case GuiControlState::NORMAL: render->DrawTexture(textureIdle, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::FOCUSED: render->DrawTexture(textureFocused, bounds.x, bounds.y, NULL);;
            break;
        case GuiControlState::PRESSED: render->DrawTexture(texturePressed, bounds.x, bounds.y, NULL);
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
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
        case GuiControlState::FOCUSED: render->DrawRectangle(bounds, 255, 255, 0, 255);
            break;
        case GuiControlState::PRESSED: render->DrawRectangle(bounds, 0, 255, 255, 255);
            break;
        case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
            break;
        default:
            break;
        }
    }

    return false;
}
