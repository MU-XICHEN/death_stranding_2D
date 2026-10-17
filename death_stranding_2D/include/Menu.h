#pragma once

#include "Entity.h"

class Menu : public Entity
{
public:
    using Entity::Entity;

    void update(World &world) override;
    void render(World &world) override;

private:
    bool started_ = false;

    void handleEvents(World &world) override;
};
