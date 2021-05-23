#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "SDL/include/SDL_gamecontroller.h"
#include "SDL/include/SDL_haptic.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50


#define MAX_KEYS 256
#define MAX_PADS 4

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct GamePad
{
public:
	//Input data
	bool start, back, guide;
	bool x, y, a, b, l1, r1, l3, r3;
	bool up, down, left, right;
	float l2, r2;
	float l_x, l_y, r_x, r_y, l_dz, r_dz;

	//Controller data
	bool enabled;
	int index;
	SDL_GameController* controller;
	SDL_Haptic* haptic;

	//Rumble controller
	int rumble_countdown;
	float rumble_strength;

	GamePad()
	{
		start = false;
		back = false;
		guide = false;

		x = false; 
		y = false;
		a = false;
		b = false;
		l1 = false;
		r1 = false;
		l3 = false;
		r3 = false;

		up = false;
		down = false;
		left = false;
		right = false;

		l2 = 0;
		r2 = 0;

		l_x = 0;
		l_y = 0;
		r_x = 0;
		r_y = 0;
		l_dz = 0;
		r_dz = 0;

		//Controller data
		enabled = false;
		index = 0;
		controller = nullptr;
		haptic = nullptr;

		//Rumble controller
		rumble_countdown = 0;
		rumble_strength = 0;
	}
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	void HandleDeviceConnection(int index);

	void HandleDeviceRemoval(int index);

	void UpdateGamepadsInput();

	bool ShakeController(int id, int duration, float strength = 0.5f);
	const char* GetControllerName(int id) const;

private:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;

	// An array to fill in the state of all the keyboard keys

public:

	KeyState keys[MAX_KEYS] = { KEY_IDLE };
	GamePad pads[MAX_PADS];

};

#endif // __INPUT_H__