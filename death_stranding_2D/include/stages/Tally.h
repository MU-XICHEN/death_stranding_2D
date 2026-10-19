#pragma once

#include "entities/Entity.h"
#include "stages/Stage.h"

class Tally : public Entity
{
public:
    using Entity::Entity;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void handleEvents(World &world);
};

class TallyStage : public Stage
{
public:
    TallyStage()
    {
        Tally *tally = new Tally(0, 0, 50, 50);
        pushEntity(tally);
    };
    ~TallyStage() = default;
};