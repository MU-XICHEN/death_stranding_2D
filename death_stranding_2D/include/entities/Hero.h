#pragma once

#include "entities/Character.h"

class Hero : public Character
{
public:
    using Character::Character;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void handleEvents(World &world);
};