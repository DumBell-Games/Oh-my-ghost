#include "App.h"
#include "Input.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include <memory>
#include <stdexcept>

#define MAX_KEYS 300

Input::Input() : Module()
{
	name.Create("input");

	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
Input::~Input()
{
	delete[] keyboard;
}

// Called before render is available
bool Input::Awake(pugi::xml_node config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
		LOG("SDL_GAMECONTROLLER could not initialize! SDL Error: %s\n", SDL_GetError());
		ret = false;
	}

	if(ret && SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// TODO Asignacion de teclas personalizadas



	return ret;
}

// Called before the first frame
bool Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool Input::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = keyboardRaw = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if(mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouseButtons[event.button.button - 1] = KEY_DOWN;
				//LOG("Mouse button %d down", event.button.button-1);
			break;

			case SDL_MOUSEBUTTONUP:
				mouseButtons[event.button.button - 1] = KEY_UP;
				//LOG("Mouse button %d up", event.button.button-1);
			break;

			case SDL_MOUSEMOTION:
				int scale = app->win->GetScale();
				mouseMotionX = event.motion.xrel / scale;
				mouseMotionY = event.motion.yrel / scale;
				mouseX = event.motion.x / scale;
				mouseY = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);

			break;

			case SDL_CONTROLLERBUTTONDOWN:

				break;

			case SDL_CONTROLLERAXISMOTION:

				break;

			case SDL_CONTROLLERDEVICEADDED:
				AddController(event.cdevice.which);
			break;
			case SDL_CONTROLLERDEVICEREMOVED:
				if (!controllers.empty()) {
					for (size_t i = 0; i < controllers.size(); i++)
					{
						if (event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers[i].get()))) {
							controllers[i].reset();
						}
					}
				}
			break;
		}
	}

	return true;
}

// Called before quitting
bool Input::CleanUp()
{
	keyboardRaw = nullptr; // No hace falta borrarlo, es gestionado por SDL
	controllers.clear(); // No deberia hacer falta borrar los punteros uno a uno, gestionado por smart pointers
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void Input::UpdateBindings()
{
	for (size_t i = 0; i < bindings.size(); i++)
	{
		bindings[i].Update(this);
	}
}

void Input::FindControllers() {
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			AddController(i);
		}
	}
}

const ControlBinding& Input::GetBind(ControlID id)
{
	// TODO (Roger) limpieza (borra el chequeo de id)
	if (id < 0 || id >= bindings.size()) { LOG("Comprueba la configuracion de controles, aqui faltan controles por configurar."); return bindings[0]; }
	return bindings[id];
}

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

void Input::GetMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}

void Input::GetMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

void ControlBinding::Update(Input* input)
{
	// TODO (Roger) limpiar esta funcion

	// Si este binding no es un eje, no recoge los datos relacionados con ejes para ahorrar calculos con coma flotante
	// key > 0 es para comprobar que este asignado (0 es que no esta asignado o es una tecla desconocida)
	bool kP = posKey > 0 ? input->GetKeyRaw(posKey) : false;
	bool kN;
	SDL_GameController* c = input->GetController();
	bool bP = false, bN = false;
	if (c) {
		// boton >= 0 es para comprobar que este asignado (-1 si no está asignado o es un boton invalido)
		if (posButton >= 0) bP = SDL_GameControllerGetButton(c, posButton);
	}

	if (isAxisControl) {
		kN = negKey > 0 && isAxisControl ? input->GetKeyRaw(negKey) : false;

		Sint16 axisTemp = 0;
		if (c) {
			if (negButton >= 0) bN = SDL_GameControllerGetButton(c, negButton);
			axisTemp = SDL_GameControllerGetAxis(c, axis);
		}

		if (axisTemp == 0) {
			axisVal = -kN - bN + kP + bP; // Ambos botones pulsados = 0 movimiento, gana el lado que tenga mas "potencia"
		}
		else
		{
			axisVal = axisTemp / 32767.0f;
		}

		// Clamp function
		axisVal = MAX(-maxVal, MIN(maxVal, axisVal));

	}else if (kP || bP) {
		state = state == KEY_IDLE ? KEY_DOWN : KEY_REPEAT;
	}
	else state = KEY_UP;


}
