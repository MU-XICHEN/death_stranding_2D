#pragma once

#include "entities/Entity.h"

// function as viewport for the game world
// render environment while updating the viewport
class Environment : public Entity
{

public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
};