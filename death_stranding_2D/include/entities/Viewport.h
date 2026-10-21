#pragma once

#include "entities/Entity.h"

// function as viewport for the game world
// render viewport while updating the viewport
class Viewport : public Entity
{

public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
};