#pragma once

#include "entities/Entity.h"
#include "GamesEngineeringBase.h"
#include "core/GameBase.h"

class TileBlock : public Entity
{
public:
    TileBlock() : Entity(0, 0, 0, 0) {}
    using Entity::Entity;

    void update(World &world, Layer &layer) override {}
    void render(World &world, Layer &layer) override {}
};
