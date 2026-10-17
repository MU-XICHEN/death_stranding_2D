#pragma once

#include "Entity.h"

class Tally : public Entity
{
public:
    using Entity::Entity;

    void update(World &world) override;
    void render(World &world) override;

private:
    void handleEvents(World &world) override;
};
