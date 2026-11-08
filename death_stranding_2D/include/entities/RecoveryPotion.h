#pragma once

#include "entities/Potion.h"

class RecoveryPotion : public Potion
{
public:
    RecoveryPotion(Hero &hero, float x, float y)
        : Potion(hero, x, y, POTION_SIZE, POTION_SIZE)
    {
        GEB::Pair<int, float> effects[] = {
            {State::HEALTH, 50.0f},
            {State::MANA, 50.0f},
        };
        effectState_.initialize(effects, 2);
    }
};
