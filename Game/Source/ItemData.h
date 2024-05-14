#pragma once
#include "SString.h"

class Personatge;

class ItemData
{
public:

	ItemData(SString _name = "Nothing");

	void UseOn(Personatge* p);

public:

	SString name;
	int amount = 0;

	int healPow = 0;

};

