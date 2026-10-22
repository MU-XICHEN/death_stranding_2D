#pragma once

#include "stages/Stage.h"
#include "entities/ItemEntity.h"
#include "world/Grid.h"

class PlayStage : public Stage
{
public:
    PlayStage();
    virtual ~PlayStage() = default;

    void pushEntityToItemEntityList(ItemEntity *itemEntity);
    void updateEntities(World &world, Layer &layer) override;
    void renderEntities(World &world, Layer &layer) override;

    GridSystem &gridSystem() { return gridSystem_; }

private:
    GridSystem gridSystem_;
};
