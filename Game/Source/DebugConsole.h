#pragma once
#include "Module.h"
#include <functional>
#include "SDL/include/SDL_pixels.h"

using ParamFunc_f = std::function<void(std::vector<std::string>)>;
class DebugCmd
{
public:

	DebugCmd(const char* _command, const char* _description, const char* _cmdFormat, ParamFunc_f func) : command(_command), description(_description), cmdFormat(_cmdFormat), function(func)
	{}

	void operator()(std::vector<std::string> args) { function(args); }

	std::string command;
	std::string description;
	std::string cmdFormat;
	ParamFunc_f function;
};


class DebugConsole : public Module
{
public:

	DebugConsole(bool startEnabled = true);

	~DebugConsole();

	bool Awake(pugi::xml_node config) override;

	bool Start() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	bool AddCommand(SString cmd, const char* description, const char* format, ParamFunc_f listener);

	void RemoveCommand(const char* cmd);

private:

	void ToggleConsole();

	void HandleCommand();

	std::vector<std::string> GetArguments(std::string str, char separator);

private:

	SDL_Color textColor;
	SDL_Color bgColor;

	std::vector<DebugCmd> commandList;

	std::string input;

	bool show = false;

};

