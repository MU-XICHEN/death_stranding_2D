#pragma once

#include "entities/Entity.h"
#include "stages/Stage.h"

class MenuStage : public Stage
{
public:
    MenuStage() = default;
    ~MenuStage() = default;
    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
};