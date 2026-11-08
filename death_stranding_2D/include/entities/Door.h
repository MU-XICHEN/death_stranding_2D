#pragma once

#include "entities/Entity.h"

class Hero;

class Door : public Entity
{
public:
    Door(Hero &hero, float x, float y);
    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
    bool unlocked() const { return unlocked_; }

private:
    Hero &hero_;
    bool unlocked_ = false;
};
