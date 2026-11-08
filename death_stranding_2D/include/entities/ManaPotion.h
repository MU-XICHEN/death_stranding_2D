#pragma once

#include "entities/Potion.h"

class ManaPotion : public Potion
{
public:
    ManaPotion(Hero &hero,
               float x,
               float y,
               int width = POTION_SIZE,
               int height = POTION_SIZE)
        : Potion(hero, x, y, width, height)
    {
        GEB::Pair<int, float> newProp[] = {{State::MANA, 50.0f}};
        int size = sizeof(newProp) / sizeof(newProp[0]);
        effectState_.initialize(newProp, size);
    }
    ~ManaPotion() = default;
};
