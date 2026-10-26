#pragma once

#include "stages/Stage.h"
#include "entities/ItemEntity.h"
#include "world/Grid.h"
#include "map/Map.h"

class PlayStage : public Stage
{
public:
    PlayStage();
    virtual ~PlayStage() = default;

    void addEntityToGridSystem(ItemEntity *itemEntity);
    void updateEntities(World &world, Layer &layer) override;
    void renderEntities(World &world, Layer &layer) override;

    GridSystem &gridSystem() { return gridSystem_; }
    Map &map() { return map_; }

private:
    GridSystem gridSystem_;
    Map map_;
};
