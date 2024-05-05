#include "DebugConsole.h"
#include "DebugCommands.h"

#include "App.h"
#include "Input.h"
#include <algorithm>
#include "Log.h"

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
	commandList.push_back(DebugCmd("warpto", "Teletransporta al jugador al mapa (y opcionalmente entrada) especificados", "warpto mapId [doorId]", WarpTo));

	return true;
}

bool DebugConsole::Update(float dt)
{
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
		}
	}


	return true;
}

bool DebugConsole::PostUpdate()
{
	return true;
}

bool DebugConsole::CleanUp()
{
	return true;
}

bool DebugConsole::AddCommand(SString cmd, ParamFunc_f listener)
{
	for (size_t i = 0; i < commandList.size(); i++)
	{
		if (cmd == commandList[i].command)
			return false;
	}
	commandList.push_back({ cmd.GetString(),listener});
	return true;
}

void DebugConsole::RemoveCommand(const char* cmd)
{
	for (size_t i = 0; i < commandList.size(); i++)
	{
		if (cmd == commandList[i].command)
			commandList.erase(commandList.begin() + i);
	}
}

void DebugConsole::ToggleConsole()
{
	show = !show;
	app->input->ResetText();
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
				commandList[i](args);
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
