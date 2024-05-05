#include "DebugConsole.h"

DebugConsole::DebugConsole(bool startEnabled) : Module(startEnabled)
{
	name.Create("debugconsole");
}

DebugConsole::~DebugConsole()
{
}

bool DebugConsole::Awake(pugi::xml_node config)
{
	return false;
}

bool DebugConsole::Start()
{
	return false;
}

bool DebugConsole::Update(float dt)
{
	
	return false;
}

bool DebugConsole::PostUpdate()
{
	return false;
}

bool DebugConsole::CleanUp()
{
	return false;
}
