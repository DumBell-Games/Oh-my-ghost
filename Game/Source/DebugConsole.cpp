#include "DebugConsole.h"
#include "DebugCommands.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "PauseMenu.h"

#include "Log.h"
#include <algorithm>
#include <Optick/include/optick.h>

DebugConsole::DebugConsole(bool startEnabled) : Module(startEnabled)
{
	name.Create("debugconsole");
}

DebugConsole::~DebugConsole()
{
}

bool DebugConsole::Awake(pugi::xml_node config)
{
	return true;
}

bool DebugConsole::Start()
{
	AddCommand("help", "Muestra la lista de comandos disponibles", "help", [this](std::vector<std::string> args) {DisplayHelp(this, args); });

	return true;
}

bool DebugConsole::Update(float dt)
{
	OPTICK_EVENT()

	if (app->input->GetButton(DEBUG_CONSOLE) == KEY_DOWN) {
		ToggleConsole();
	}

	if (show) {
		input = app->input->GetText();

		if (app->input->GetKey(SDL_Scancode::SDL_SCANCODE_RETURN) == KEY_DOWN) {
			
			try {
				HandleCommand();

			}
			catch (std::invalid_argument e) {
				LOG("Argumento invalido: %s", e.what());
			}
			//TODO añadir handling de otras clases de excepcion
		}
	}


	return true;
}

bool DebugConsole::PostUpdate()
{
	if (show) {
		//Input box
		app->render->DrawRectangle({ 0,0,app->render->camera.w,64 }, 0, 0, 0, 192, true, false);
		app->render->DrawText(input.c_str(), 10, 8, 0, 48, {255,255,255,255});

	}

	return true;
}

bool DebugConsole::CleanUp()
{
	return true;
}

bool DebugConsole::AddCommand(SString cmd, const char* description, const char* format, ParamFunc_f listener)
{
	for (size_t i = 0; i < commandList.size(); i++)
	{
		if (cmd == commandList[i].command) {
			LOG("Command \"%s\" already exists!", cmd.GetString());
			return false;
		}
	}
	commandList.push_back({ cmd.GetString(), description, format,listener});
	LOG("Added command \"%s\"", cmd.GetString());
	return true;
}

void DebugConsole::RemoveCommand(const char* cmd)
{
	for (size_t i = 0; i < commandList.size(); i++)
	{
		if (commandList[i].command == cmd) {
			commandList.erase(commandList.begin() + i);
			LOG("Removed command \"%s\"", cmd);
		}
	}
}

void DebugConsole::ToggleConsole()
{
	show = !show;
	app->input->ResetText();
	//app->entityManager->Pause();
	if (show)
		SDL_StartTextInput();
	else
		SDL_StopTextInput();
}

void DebugConsole::HandleCommand()
{
	//Convierte todos los caracteres del comando a minusculas
	std::transform(input.begin(), input.end(), input.begin(), tolower);
	std::vector<std::string> args = GetArguments(input, ' ');
	
	if (!args.empty())
	{
		std::string command = args.front();
		for (size_t i = 0; i < commandList.size(); i++)
		{
			if (commandList[i].command == command) {
				LOG("Executing command\"%s\"", input.c_str());
				commandList[i](args);
				ToggleConsole(); // Cierra la consola una vez ejecutado el comando (si se ha cerrado desde el propio comando, vuelve a abrir la consola)
			}
		}
	}


	app->input->ResetText();
}

std::vector<std::string> DebugConsole::GetArguments(std::string str, char separator)
{
	std::vector<std::string> args;
	std::stringstream ss(input);
	std::string s;

	while (getline(ss, s, ' ')) {
		args.push_back(s);
	}
	return args;
}

void DisplayHelp(DebugConsole* console, std::vector<std::string> args)
{
	LOG("WIP");
}
