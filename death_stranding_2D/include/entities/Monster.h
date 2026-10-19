#pragma once

#include "entities/Entity.h"
#include "core/GameBase.h"

class Monster : public Character
{
public:
    Monster(float x, float y, int width, int height, float distance, float speed = PIXEL_PER_SEC)
        : Character(x, y, width, height), dangerDistance_(distance), speed_(speed) {}

    virtual ~Monster() = default;

protected:
    static GEB::Direction calculateDirectionToHero(GEB::Position monsterCenter, GEB::Position heroCenter);

    void awaken() { awakened_ = true; }
    void directToHero(World &world, Layer &layer);

    bool awakened_ = false;
    float dangerDistance_;
    float speed_;
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