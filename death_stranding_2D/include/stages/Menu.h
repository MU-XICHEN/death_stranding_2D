#pragma once

#include "entities/Entity.h"
#include "stages/Stage.h"

class Menu : public Entity
{
public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    bool started_ = false;

    void handleEvents(World &world);
};

class MenuStage : public Stage
{
public:
    MenuStage()
    {
        Menu *menu = new Menu(0, 0, 50, 50);
        pushEntity(menu);
    };
    ~MenuStage() = default;
};
