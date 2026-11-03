#include "skills/AttackCollision.h"
#include "skills/AttackInfo.h"
#include "layers/MainLayer.h"

void AttackCollision::tryHit(World &world, Layer &layer)
{
    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto *gridSystem = playStage.gridSystem();
    if (!gridSystem)
        return;

    if (!preTryHit(world, layer))
        return;

    const auto &atkInfo = attackInfo();

    const auto point = atkInfo.originPoint();
    int width = static_cast<int>(atkInfo.hitBoxWidth());
    int height = static_cast<int>(atkInfo.hitBoxHeight());

    AABB aabb{
        point.x,
        point.y,
        width,
        height};

    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem->getCandidateListByAABB(owner_, aabb, candidateItemList);

    if (!(candidateItemList.size() > 0))
        return;

    for (int i = 0; i < candidateItemList.size(); ++i)
    {
        if (!candidateItemList[i])
            continue;

        auto *entity = candidateItemList[i];
        if (!entity)
            continue;

        if (aabb.intersects(*entity))
        {
            auto *item = dynamic_cast<ItemEntity *>(entity);
            if (!item)
                return;
            hitTarget(*item, atkInfo);
        }
    }
}
