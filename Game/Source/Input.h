#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include <vector>
#include <memory>

#include "SDL/include/SDL.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;
class Input;

// Atajo para smart pointers de tipo SDL_GameController
using unique_gameController_t = std::unique_ptr<SDL_GameController, decltype(&SDL_GameControllerClose)>;

// Identificadores de controles deben ser añadidos aqui
enum ControlID {
	NONE = -1,
	CONFIRM,
	BACK,
	PAUSE,

	ID_COUNT
};

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

// Contains information about a button and/or axis binding
struct ControlBinding {
public:

	ControlBinding(ControlID _id, SDL_Scancode _posKey, SDL_GameControllerButton _posButton, SDL_Scancode _negKey, SDL_GameControllerButton _negButton, SDL_GameControllerAxis _axis, float _maxVal) : id(_id), posKey(_posKey), posButton(_posButton), negKey(_negKey), negButton(_negButton), axis(_axis), maxVal(_maxVal)
	{
		// If any axis-related values is assigned, this binding will update axis data
		isAxisControl = (axis != SDL_CONTROLLER_AXIS_INVALID
			|| negKey != SDL_SCANCODE_UNKNOWN
			|| negButton != SDL_CONTROLLER_BUTTON_INVALID);
			
	}

	void Update(Input* input);

	void AssignPosKey(SDL_Scancode id) { posKey = id; }
	void AssignPosButton(SDL_GameControllerButton id) { posButton = id; }
	void AssignNegKey(SDL_Scancode id) { negKey = id; }
	void AssignNegButton(SDL_GameControllerButton id) { negButton = id; }
	void AssignAxis(SDL_GameControllerAxis id) { axis = id; }
	void AssignMaxVal(float val) { maxVal = abs(val); axisVal = MAX(-maxVal, MIN(maxVal, axisVal)); }

private:

	bool isAxisControl;

	ControlID id = ControlID::NONE;
	SDL_Scancode posKey = SDL_Scancode::SDL_SCANCODE_UNKNOWN;
	SDL_GameControllerButton posButton = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID;
	SDL_Scancode negKey = SDL_Scancode::SDL_SCANCODE_UNKNOWN;
	SDL_GameControllerButton negButton = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID;
	SDL_GameControllerAxis axis = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_INVALID;

	KeyState state = KeyState::KEY_IDLE;
	float axisVal = 0.0f;
	float maxVal = 1.0f;
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Actualiza los controles asignados
	void UpdateBindings();

	// Encuentra todos los mandos compatibles conectado al pc
	void FindControllers();

	// Añade el mando con el id proporcionado a la lista de mandos detectados
	void AddController(Sint32 id) {
		controllers.push_back(unique_gameController_t(SDL_GameControllerOpen(id), SDL_GameControllerClose));
	}

	// Por defecto devuelve el primer mando, o nullptr si no hay mandos conectados
	SDL_GameController* GetController(uint id = 0) { return (id < controllers.size()) ? controllers[id].get() : nullptr; }

	// Check key states
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	bool GetKeyRaw(int id) const
	{
		return keyboardRaw[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Devuelve el estado de un boton, ya sea de teclado o mando
	const ControlBinding& GetBind(ControlID id);

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

private:
	bool windowEvents[WE_COUNT];
	const Uint8* keyboardRaw;
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	std::vector<unique_gameController_t> controllers;
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;

	std::vector<ControlBinding> bindings;

};

#endif // __INPUT_H__