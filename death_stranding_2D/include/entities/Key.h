#pragma once

#include "entities/Entity.h"

class Hero;
class Map;

class Key : public Entity
{
public:
    Key(Hero &hero, const Map &map);
    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void onCollected();
    Hero &hero_;
    bool collected_ = false;
};
