#pragma once

#include "entities/Entity.h"
#include "stages/Stage.h"

class TallyStage : public Stage
{
public:
    TallyStage() = default;
    ~TallyStage() = default;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
};