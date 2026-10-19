#pragma once

#include <Entity.h>

class Monster : public Entity
{
public:
    using Entity::Entity;
    virtual ~Monster() = default;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void directToHero(World &world, Layer &layer);

    float awareDistance_;
};

class Ghost : public Monster
{
public:
private:
    bool visible_;
};

class Beast : public Monster
{
};