#include "skills/AttackCollision.h"
#include "skills/AttackInfo.h"
#include "layers/MainLayer.h"

void AttackCollision::tryBegin(World &world, Layer &layer)
{
    if (!owner_)
        return;
    if (!owner_->active())
        return;
    begin();
}

void AttackCollision::tryHit(World &world, Layer &layer)
{
    if (!owner_)
        return;
    if (!owner_->active())
        return;

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

    for (int i = 0; i < candidateItemList.size(); ++i)
    {
        auto *targetItem = dynamic_cast<ItemEntity *>(candidateItemList[i]);
        if (!targetItem)
            continue;
        if (!targetItem->active())
            continue;
        if (targetItem == owner_)
            continue;
        if (!aabb.intersects(*targetItem))
            continue;

        auto targetId = reinterpret_cast<std::uintptr_t>(targetItem);
        if (hitTargets_.contains(targetId))
            continue;

        hitTarget(*targetItem, atkInfo);
        hitTargets_.insert(targetId);

        // reset the elapsed time for the next contact cooldown.
        elapsedTime_ = 0.0f;
    }
}
