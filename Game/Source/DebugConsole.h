#pragma once
#include "Module.h"
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

private:


};

