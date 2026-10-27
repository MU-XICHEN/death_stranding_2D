#pragma once

#include "entities/Entity.h"
#include "stages/Stage.h"

class IntroStage : public Stage
{
public:
    IntroStage() = default;
    ~IntroStage() = default;
    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
};