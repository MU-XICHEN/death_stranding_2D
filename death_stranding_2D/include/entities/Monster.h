#pragma once

#include "core/GameBase.h"
#include "entities/Character.h"

class Monster : public Character
{
public:
    Monster(float x, float y, int width, int height, float distance, float speed = PIXEL_PER_SEC)
        : Character(x, y, width, height, speed), dangerDistance_(distance) {}

    virtual ~Monster() = default;

protected:
    static GEB::Direction calculateDirectionToHero(GEB::Position monsterCenter, GEB::Position heroCenter);

    void awaken() { awakened_ = true; }
    void directToHero(World &world, Layer &layer);

    bool awakened_ = false;
    float dangerDistance_;
};

class Beast : public Monster
{
};

class Ghost : public Monster
{
public:
private:
    bool visible_;
};