#pragma once
#include <string>

class Personatge;

// Clase base para estados alterados
class StatusEffect
{
public:
	
	StatusEffect() : name("PH"), duration(0)
	{}

	StatusEffect(const char* _name, int _duration) : name(_name), duration(_duration)
	{}

	~StatusEffect()
	{}

	// Aplica el estado alterado en ese turno. Devuelve false si el estado alterado ha expirado y debe quitarse del personaje
	virtual bool Tick(Personatge* p) = 0;

	// Aumenta o reduce el ataque/defensa para el ataque que se esta calculando. Devuelve true si el ataque es interrumpido por el estado alterado
	virtual bool ApplyToAttack(float& defMultiplier, float& atkMultiplier) = 0;

	virtual std::string GetApplicationMsg() const { return (applicationMsg+name); }

	std::string name;
	std::string applicationMsg = "Se ha aplicado el estado alterado ";
	int duration;
	int timePassed = 0;
};

