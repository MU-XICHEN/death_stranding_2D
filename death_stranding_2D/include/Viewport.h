#pragma once

#include "Entity.h"

class Viewport : public Entity
{

public:
    using Entity::Entity;

    void update(World &world) override;
    void render(World &world) override;
};