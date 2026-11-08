#pragma once

#include "Entity.h"
#include "ItemEntity.h"
#include "skills/State.h"
class World;
class Layer;
class Hero;

class Potion : public ItemEntity
{
public:
    Potion(Hero &hero, float x, float y, int width, int height);
    virtual void update(World &, Layer &) override;
    void render(World &world, Layer &layer) override;

    const State &effectState() const { return effectState_; }

protected:
    State effectState_;

private:
    Hero &hero_;
};
