#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "Point.h"
#include <vector>
#include <memory>

#include "SDL/include/SDL.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

#define JOYSTICK_MAX 32767
#define DEAD_ZONE 0.05f
#define DEAD_ZONE_INT_MAX (JOYSTICK_MAX-100)
#define DEAD_ZONE_INT_MIN 100

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
	MOVE_HORIZONTAL,
	MOVE_VERTICAL,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	APP_EXIT,
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

	ControlBinding()
	{
		SetDeadZone(DEAD_ZONE);
	}

	ControlBinding(SDL_Scancode _posKey, SDL_GameControllerButton _posButton, SDL_Scancode _negKey, SDL_GameControllerButton _negButton, SDL_GameControllerAxis _axis, float _maxVal, float _deadZone) : posKey(_posKey), posButton(_posButton), negKey(_negKey), negButton(_negButton), axis(_axis), maxVal(_maxVal)
	{
		// If any axis-related values is assigned, this binding will update axis data
		isAxisControl = (axis != SDL_CONTROLLER_AXIS_INVALID
			|| negKey != SDL_SCANCODE_UNKNOWN
			|| negButton != SDL_CONTROLLER_BUTTON_INVALID);
			
		SetDeadZone(_deadZone);
	}

	void Update(Input* input);

	void Map(SDL_Scancode& _pKey, SDL_Scancode& _nKey, SDL_GameControllerButton& _pBut, SDL_GameControllerButton& _nBut, bool _isAxis, SDL_GameControllerAxis _axis, ControlID _bind);
	ControlBinding& SetPKey(SDL_Scancode id) { posKey = id; return *this; }
	ControlBinding& SetPButton(SDL_GameControllerButton id) { posButton = id; return *this; }
	ControlBinding& SetNKey(SDL_Scancode id) { negKey = id; return *this; }
	ControlBinding& SetNButton(SDL_GameControllerButton id) { negButton = id; return *this; }
	ControlBinding& SetAxisControl(bool val) { isAxisControl = val; axisVal = 0; return *this; }
	ControlBinding& SetAxis(SDL_GameControllerAxis id) { axis = id; return *this; }
	ControlBinding& SetMaxVal(float val) { maxVal = abs(val); axisVal = MAX(-maxVal, MIN(maxVal, axisVal)); return *this; }
	ControlBinding& SetDeadZone(float val) { deadZone = MAX(DEAD_ZONE_INT_MIN, MIN(DEAD_ZONE_INT_MAX, abs(val)*JOYSTICK_MAX)); return *this; }
	ControlBinding& SetBind(ControlID id) { bind = id; return *this; }
	ControlBinding& SetIsPositive(bool val) { isPositive = val; return *this; }

	// Devuelve el estado del control, combinando teclado y mando
	KeyState State() const { return state; }
	// Si isAxisControl == false siempre devolvera 0
	float Axis() const { return axisVal; }

	void LogData(ControlID id) const;

public:

	bool isAxisControl = false;
	ControlID bind = ControlID::NONE;
	bool isPositive = false;

	// Teclas y botones
	SDL_Scancode posKey = SDL_Scancode::SDL_SCANCODE_UNKNOWN;
	SDL_GameControllerButton posButton = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID;

	// Solo para ejes
	SDL_Scancode negKey = SDL_Scancode::SDL_SCANCODE_UNKNOWN;
	SDL_GameControllerButton negButton = SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID;
	SDL_GameControllerAxis axis = SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_INVALID;

	// Valores
	KeyState state = KeyState::KEY_IDLE;
	float axisVal = 0.0f;
	float maxVal = 1.0f;
	int deadZone; // Set in constructor
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

	// Por defecto devuelve el primer mando, o nullptr si no hay mandos conectados
	SDL_GameController* GetController(uint id = 0) { return (id < controllers.size()) ? controllers[id].get() : nullptr; }

	// Check key states
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	// Devuelve el estado del boton cuyo id coincide con el proporcionado
	KeyState GetButton(ControlID id);

	// Devuelve un eje
	float GetAxis(ControlID x);
	// Devuelve dos ejes
	fPoint GetAxis(ControlID x, ControlID y);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	bool SaveBindings();

private:

	bool GetKeyRaw(int id) const
	{
		return keyboardRaw[id];
	}

	// Devuelve el estado de un boton, ya sea de teclado o mando
	const ControlBinding& GetBind(ControlID id);

	// Actualiza los controles asignados
	void UpdateBindings();

	// Encuentra todos los mandos compatibles conectado al pc
	void FindControllers();

	// Añade el mando con el id proporcionado a la lista de mandos detectados
	void AddController(Sint32 id);

private:

	SString filePath;

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

	friend ControlBinding;

};

#endif // __INPUT_H__