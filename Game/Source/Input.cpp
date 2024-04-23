#include "App.h"
#include "Input.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#define MAX_KEYS 300

Input::Input(bool startEnabled) : Module(startEnabled)
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

	filePath = config.attribute("bindingsFile").as_string("controls.xml");

	for (size_t i = 0; i < ControlID::ID_COUNT; i++)
	{
		bindings.push_back(ControlBinding());
	}

	//Default (por si hay errores en el archivo de configuracion)
	bindings[CONFIRM]
		.SetPButton(SDL_CONTROLLER_BUTTON_A)
		.SetPKey(SDL_SCANCODE_SPACE);

	bindings[BACK]
		.SetPButton(SDL_CONTROLLER_BUTTON_B)
		.SetPKey(SDL_SCANCODE_LSHIFT);

	bindings[PAUSE]
		.SetPButton(SDL_CONTROLLER_BUTTON_START)
		.SetPKey(SDL_SCANCODE_P);

	bindings[MOVE_HORIZONTAL]
		.SetAxisControl(true)
		.SetAxis(SDL_CONTROLLER_AXIS_LEFTX)
		.SetPButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
		.SetNButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT)
		.SetPKey(SDL_SCANCODE_D)
		.SetNKey(SDL_SCANCODE_A);

	bindings[MOVE_VERTICAL]
		.SetAxisControl(true)
		.SetAxis(SDL_CONTROLLER_AXIS_LEFTY)
		.SetPButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN)
		.SetNButton(SDL_CONTROLLER_BUTTON_DPAD_UP)
		.SetPKey(SDL_SCANCODE_S)
		.SetNKey(SDL_SCANCODE_W);

	bindings[APP_EXIT]
		.SetPButton(SDL_CONTROLLER_BUTTON_BACK)
		.SetPKey(SDL_SCANCODE_ESCAPE);

	pugi::xml_document controlsDoc;
	pugi::xml_parse_result result = controlsDoc.load_file(filePath.GetString());
	if (result)
	{
		LOG("control bindings file parsed without errors");

		for (pugi::xml_node bindingNode = controlsDoc.child("controls").child("binding"); bindingNode; bindingNode = bindingNode.next_sibling("binding"))
		{
			ControlID id = (ControlID)bindingNode.attribute("id").as_int(ControlID::NONE);
			SDL_Scancode pK = (SDL_Scancode)bindingNode.attribute("pKey").as_int(SDL_Scancode::SDL_SCANCODE_UNKNOWN);
			SDL_Scancode nK = (SDL_Scancode)bindingNode.attribute("nKey").as_int(SDL_Scancode::SDL_SCANCODE_UNKNOWN);
			SDL_GameControllerButton pB = (SDL_GameControllerButton)bindingNode.attribute("pBut").as_int(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID);
			SDL_GameControllerButton nB = (SDL_GameControllerButton)bindingNode.attribute("nBut").as_int(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID);
			bool isAxis = bindingNode.attribute("isAxis").as_bool(false);
			SDL_GameControllerAxis axis = (SDL_GameControllerAxis)bindingNode.attribute("axis").as_int(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_INVALID);
			float deadzone = bindingNode.attribute("deadzone").as_float(DEAD_ZONE);
			ControlID bind = (ControlID)bindingNode.attribute("bind").as_int(ControlID::NONE);
			bool isPositive = bindingNode.attribute("isPositive").as_bool(false);

			//bindings[id].Map(pK, nK, pB, nB, isAxis, axis, bind);
			bindings[id].SetPKey(pK).SetNKey(nK).SetPButton(pB).SetNButton(nB).SetAxisControl(isAxis).SetAxis(axis).SetDeadZone(deadzone).SetBind(bind).SetIsPositive(isPositive);
		}
	}
	else
	{
		LOG("Error loading control bindings: %s", result.description());
		if (result.status == pugi::status_file_not_found) {
			LOG("Creating bindings file...");
			SaveBindings();
		}
	}

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

	UpdateBindings();

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

			case SDL_MOUSEMOTION: {
				int scale = app->win->GetScale();
				mouseMotionX = event.motion.xrel / scale;
				mouseMotionY = event.motion.yrel / scale;
				mouseX = event.motion.x / scale;
				mouseY = event.motion.y / scale;
				//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			}
			break;

			case SDL_CONTROLLERDEVICEADDED:
				AddController(event.cdevice.which);
			break;

			case SDL_CONTROLLERDEVICEREMOVED:
				for (size_t i = 0; i < controllers.size(); i++)
				{
					if (event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controllers[i].get()))) {
						unique_gameController_t sink = std::move(controllers[i]);
					}
				}
				FindControllers();
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

KeyState Input::GetButton(ControlID id)
{
	return GetBind(id).State();
}

float Input::GetAxis(ControlID x)
{
	return GetBind(x).Axis();
}

fPoint Input::GetAxis(ControlID x, ControlID y)
{
	fPoint ret;
	ret.x = GetBind(x).Axis();
	ret.y = (y == NONE) ? 0 : GetBind(y).Axis();
	return ret;
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

bool Input::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

bool Input::SaveBindings()
{
	pugi::xml_document controlsDoc;
	pugi::xml_node rootNode = controlsDoc.append_child("controls");

	for (int i = 0; i < ControlID::ID_COUNT; i++)
	{
		pugi::xml_node bindNode = rootNode.append_child("binding");
		bindNode.append_attribute("id").set_value(i);
		bindNode.append_attribute("pKey").set_value(bindings[i].posKey);
		bindNode.append_attribute("nKey").set_value(bindings[i].negKey);
		bindNode.append_attribute("pBut").set_value(bindings[i].posButton);
		bindNode.append_attribute("nBut").set_value(bindings[i].negButton);
		bindNode.append_attribute("isAxis").set_value(bindings[i].isAxisControl);
		bindNode.append_attribute("axis").set_value(bindings[i].axis);
		bindNode.append_attribute("deadzone").set_value(bindings[i].deadZone/32768.0f);
		bindNode.append_attribute("bind").set_value(bindings[i].bind);
		bindNode.append_attribute("isPositive").set_value(bindings[i].isPositive);
	}
	controlsDoc.save_file(filePath.GetString());

	return true;
}

const ControlBinding& Input::GetBind(ControlID id)
{
	return bindings.at(id);
}

void Input::UpdateBindings()
{
	for (size_t i = 0; i < bindings.size(); i++)
	{
		bindings[i].Update(this);
	}
}

void Input::FindControllers() {
	controllers.clear(); // NOTE esto puede causar problemas en juegos multijugador, pero como este es de un solo jugador no pasa nada
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (SDL_IsGameController(i)) {
			AddController(i);
		}
	}
}

inline void Input::AddController(Sint32 id) {
	bool reassigned = false;
	for (size_t i = 0; i < controllers.size(); i++)
	{
		if (!controllers[i]) {
			unique_gameController_t source = unique_gameController_t(SDL_GameControllerOpen(id), SDL_GameControllerClose);
			controllers[i] = std::move(source);
			reassigned = true;
			break;
		}
	}
	if (!reassigned)
		controllers.push_back(unique_gameController_t(SDL_GameControllerOpen(id), SDL_GameControllerClose));
}

void ControlBinding::Update(Input* input)
{
	SDL_GameController* c = input->GetController();
	bool bP = false, bN = false; // Botones de mando (buttons)
	bool kP = false, kN = false; // Teclas de teclado (keys)

	// Si este binding no es un eje, no recoge los datos relacionados con ejes para ahorrar llamadas extra
	if (isAxisControl) {
		kP = input->GetKeyRaw(posKey);
		kN = input->GetKeyRaw(negKey);

		Sint16 axisTemp = 0;
		if (c) {
			bP = SDL_GameControllerGetButton(c, posButton);
			bN = SDL_GameControllerGetButton(c, negButton);
			axisTemp = SDL_GameControllerGetAxis(c, axis);
		}
		if (abs(axisTemp) < deadZone) {
			axisVal = -kN - bN + kP + bP; // Botones opuestos pulsados = 0 movimiento, gana el lado que tenga mas "potencia"
		}
		else
		{
			axisVal = axisTemp / 32767.0f;
			// TODO deadzone por config
			//if (abs(axisVal) <= DEAD_ZONE) axisVal = 0;
		}
		// Clamp function
		axisVal = MAX(-maxVal, MIN(maxVal, axisVal));

	}
	else if (bind != ControlID::NONE) {
		float axis = app->input->GetAxis(bind);
		bool check = (isPositive ? (axis > 0):(axis < 0));
		if (check)
			state = state == KEY_IDLE ? KEY_DOWN : KEY_REPEAT;
		else if (state == KEY_DOWN || state == KEY_REPEAT)
			state = KEY_UP;
		else
			state = KEY_IDLE;
	}
	else {
		kP = input->GetKeyRaw(posKey);
		if (c) {
			bP = SDL_GameControllerGetButton(c, posButton);
		}

		if (kP || bP)
			state = state == KEY_IDLE ? KEY_DOWN : KEY_REPEAT;
		else if (state == KEY_DOWN || state == KEY_REPEAT)
			state = KEY_UP;
		else
			state = KEY_IDLE;
	}

}

void ControlBinding::Map(SDL_Scancode& _pKey, SDL_Scancode& _nKey, SDL_GameControllerButton& _pBut, SDL_GameControllerButton& _nBut, bool _isAxis, SDL_GameControllerAxis _axis, ControlID _bind)
{
	posKey = _pKey;
	negKey = _nKey;
	posButton = _pBut;
	negButton = _nBut;
	isAxisControl = _isAxis;
	axis = _axis;
	bind = _bind;
}

void ControlBinding::LogData(ControlID id) const
{
	LOG("[Binding %i]\n", id);
	LOG("PosKey: %i\n", posKey);
	LOG("NegKey: %i\n", negKey);
	LOG("PosButton: %i\n", posButton);
	LOG("NegButton: %i\n", negButton);
	LOG("State: %i\n", state);
	LOG("IsAxis: %i\n", isAxisControl);
	LOG("Axis: %i\n", axis);
	LOG("AxisVal: %i\n", axisVal);
	LOG("MaxVal: %i\n", maxVal);
}
