#pragma once

#include "entities/Entity.h"
#include "stages/Stage.h"

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

class IntroStage : public Stage
{
public:
    IntroStage()
    {
        Intro *intro = new Intro(0, 0, 50, 50);
        pushEntity(intro);
    };
    ~IntroStage() = default;
};