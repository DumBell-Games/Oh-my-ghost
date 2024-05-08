#include "Poison.h"
#include "Enemic.h"

Poison::Poison() : StatusEffect("Poison", 5)
{
}

Poison::~Poison()
{
}

bool Poison::Tick(Personatge* p)
{
    p->salutActual -= p->salutTotal * ((float)power / 100.0f);
    return timePassed >= duration;
}

bool Poison::ApplyToAttack(float& defMultiplier, float& atkMultiplier)
{
    return false;
}
