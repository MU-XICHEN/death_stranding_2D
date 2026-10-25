#pragma once

#include "entities/Entity.h"
#include "GamesEngineeringBase.h"
#include "core/GameBase.h"

class Tile : public Entity
{
public:
    Tile()
        : Entity(0, 0, 0, 0) {}

    void update(World &world, Layer &layer) override {};
    void render(World &world, Layer &layer) override {};
};