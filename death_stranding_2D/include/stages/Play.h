#pragma once

#include "stages/Stage.h"
#include "entities/ItemEntity.h"

class PlayStage : public Stage
{
public:
    PlayStage();
    virtual ~PlayStage() override;

    void pushEntityToItemEntityList(ItemEntity *itemEntity);
    void updateEntities(World &world, Layer &layer) override;
    void renderEntities(World &world, Layer &layer) override;

    const GEB::Structures::Array<ItemEntity *>& itemEntityList() { return itemEntityList_; }

private:
    GEB::Structures::Array<ItemEntity *> itemEntityList_;
};
