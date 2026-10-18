#pragma once

#include "Entity.h"

class Intro : public Entity
{
public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    bool started_ = false;

    void handleEvents(World &world);
};
