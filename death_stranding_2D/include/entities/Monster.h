#pragma once

#include "entities/Entity.h"
#include "core/GameBase.h"

class Monster : public Entity
{
public:
    Monster(float x, float y, int width, int height, float awareDistance)
        : Entity(x, y, width, height), awareDistance_(awareDistance) {}

    virtual ~Monster() = default;

private:
    void directToHero(World &world, Layer &layer);

    float awareDistance_;
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