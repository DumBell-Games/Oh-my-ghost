#pragma once
#include "StatusEffect.h"

class Poison :
    public StatusEffect
{
public:

    Poison();

    ~Poison();

    bool Tick(Personatge* p) override;

    bool ApplyToAttack(float& defMultiplier, float& atkMultiplier);

private:

    // Percent HP drained
    int power = 12;

};

