#pragma once

#include "entities/Entity.h"

class Viewport : public Entity
{

public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
};