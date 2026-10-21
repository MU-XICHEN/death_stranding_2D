#pragma once

#include "entities/Character.h"

class Hero : public Character
{
public:
    Hero()
        : Character(0, 0, ROLE_SIZE, ROLE_SIZE) {}
    using Character::Character;

    void update(World &world, Layer &layer) override;

private:
    void handleEvents(World &world);
};