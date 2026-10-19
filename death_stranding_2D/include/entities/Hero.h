#pragma once

#include "entities/Entity.h"

class Hero : public Entity
{
public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void handleEvents(World &world);
};