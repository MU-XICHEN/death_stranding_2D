#pragma once

#include "entities/Character.h"

struct HeroInfo
{
    float x;
    float y;
    int width;
    int height;

    GEB::Position center() const { return {x + width / 2, y + height / 2}; }
};

class Hero : public Character
{
public:
    explicit Hero()
        : Character(0, 0, ROLE_SIZE, ROLE_SIZE) {}
    using Character::Character;

    void update(World &world, Layer &layer) override;

private:
    void handleEvents(World &world);

    HeroInfo heroInfo_;
};