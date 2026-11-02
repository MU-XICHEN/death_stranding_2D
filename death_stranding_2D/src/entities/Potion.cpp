#include "entities/Potion.h"
#include "entities/ItemEntity.h"
#include "entities/AABB.h"
#include "world/World.h"
#include "layers/MainLayer.h"

void Potion::tryApplyTo(ItemEntity *item)
{
    if (!active_)
        return;
    if (item->onUsePotion(this))
        setActive(false);
}

void Potion::update(World &world, Layer &layer)
{

    ItemEntity::coreUpdate(world, layer);
    if (!active())
        return;

    AABB aabb{
        x_,
        y_,
        width_,
        height_,
    };

    // get candidate items for collision detection
    GEB::Structures::Array<Entity *> candidateItemList;

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto *gridSystem = playStage.gridSystem();
    if (!gridSystem)
        return;

    gridSystem->getCandidateListByAABB(this, aabb, candidateItemList);

    for (int i = 0; i < candidateItemList.size(); i++)
    {
        auto *entity = candidateItemList[i];
        if (!entity)
            continue;
        if (aabb.intersects(*entity))
        {
            auto *item = dynamic_cast<ItemEntity *>(entity);
            if (!item)
                continue;
            tryApplyTo(item);
        }
    }
};
