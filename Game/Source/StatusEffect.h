#pragma once
#include <string>

// Clase base para estados alterados
class StatusEffect
{
public:
	StatusEffect(const char* _name, int _duration) : name(_name), duration(_duration)
	{}

	~StatusEffect()
	{}

	// Aplica el estado alterado en ese turno
	virtual bool Tick() = 0;

	// Aumenta o reduce el ataque/defensa para el ataque que se esta calculando
	virtual bool ApplyToAttack(float& defMultiplier, float& atkMultiplier) = 0;

	std::string name;
	int duration;
};

